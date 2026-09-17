import { composingPattern, escapeRegExp } from "./hangul";

// one entry of search.json: a post's plain text, NFC-normalized at build time
export interface SearchPost {
  number: number;
  title: string;
  text: string;
}

export interface ExcerptPart {
  text: string;
  marked: boolean;
}

export interface SearchResult {
  number: number;
  title: string;
  excerpt: ExcerptPart[];
}

interface Hit {
  start: number;
  end: number;
  word: number;
  // at the start of a word rather than inside one, as in 합을 against 조합을
  wordStart: boolean;
}

const EXCERPT_LENGTH = 160;
// how far the excerpt opens before the hit it is built around
const EXCERPT_LEAD = 40;
// how far the excerpt's ends may move to fall between words
const EXCERPT_SNAP = 15;
const LETTER = /[\p{L}\p{N}]/u;

const isLowSurrogate = (code: number): boolean =>
  code >= 0xdc00 && code <= 0xdfff;

const hitsOf = (text: string, patterns: RegExp[]): Hit[] =>
  patterns.flatMap((pattern, word) =>
    Array.from(text.matchAll(pattern), (match) => ({
      start: match.index,
      end: match.index + match[0].length,
      word,
      wordStart: match.index === 0 || !LETTER.test(text[match.index - 1]),
    })),
  );

// Korean joins words, so matching inside a word is what finds 복잡도 in 시간복잡도는; a word that starts with a query word
// still counts four times as much, which keeps 합을 above 조합을. A title match outweighs any body text, and a word
// counts less the more often it repeats
const scoreOf = (title: Hit[], body: Hit[], words: number): number => {
  let score = 0;
  for (let word = 0; word < words; word++) {
    const inTitle = title.filter((hit) => hit.word === word);
    const inBody = body.filter((hit) => hit.word === word);
    if (inTitle.length > 0) {
      score += inTitle.some((hit) => hit.wordStart) ? 20 : 10;
    }
    const weight = inBody.reduce(
      (sum, hit) => sum + (hit.wordStart ? 1 : 0.25),
      0,
    );
    score += weight / (weight + 1);
  }
  return score;
};

// the part of the text around the hits that covers the most query words, starting and ending between words
const excerptOf = (text: string, hits: Hit[], words: number): ExcerptPart[] => {
  const sorted = [...hits].sort((a, b) => a.start - b.start);
  // a window per hit, kept in one pass: a query such as ㅇ, read as any syllable starting with it, has hundreds of hits
  const inWindow = new Array<number>(words).fill(0);
  let covered = 0;
  let first = 0;
  let next = 0;
  let from = 0;
  let best = -1;
  for (const anchor of sorted) {
    const start = Math.max(0, anchor.start - EXCERPT_LEAD);
    while (next < sorted.length && sorted[next].end <= start + EXCERPT_LENGTH) {
      if (inWindow[sorted[next].word]++ === 0) {
        covered++;
      }
      next++;
    }
    while (first < next && sorted[first].start < start) {
      if (--inWindow[sorted[first].word] === 0) {
        covered--;
      }
      first++;
    }
    const value = covered * 2 + (anchor.wordStart ? 1 : 0);
    if (value > best) {
      best = value;
      from = start;
    }
  }
  // a match near the end still gets a full excerpt, taken from before it
  from = Math.max(0, Math.min(from, text.length - EXCERPT_LENGTH));
  let to = Math.min(text.length, from + EXCERPT_LENGTH);
  if (from > 0) {
    const space = text.lastIndexOf(" ", from);
    if (space !== -1 && from - space <= EXCERPT_SNAP) {
      from = space + 1;
    }
  }
  if (to < text.length) {
    const space = text.indexOf(" ", to);
    if (space !== -1 && space - to <= EXCERPT_SNAP) {
      to = space;
    }
  }
  // an end that falls inside a character written as two code units, such as an emoji, takes in the whole character
  if (from > 0 && isLowSurrogate(text.charCodeAt(from))) {
    from--;
  }
  if (to < text.length && isLowSurrogate(text.charCodeAt(to))) {
    to++;
  }

  const marks = sorted.filter((hit) => hit.start >= from && hit.end <= to);
  const parts: ExcerptPart[] = [];
  let position = from;
  for (const mark of marks) {
    const previous = parts.at(-1);
    if (mark.start < position) {
      // overlaps the mark before it, as 시간복 and 복잡도 do in 시간복잡도
      if (previous?.marked && mark.end > position) {
        previous.text += text.slice(position, mark.end);
        position = mark.end;
      }
      continue;
    }
    if (mark.start > position) {
      parts.push({ text: text.slice(position, mark.start), marked: false });
    }
    parts.push({ text: text.slice(mark.start, mark.end), marked: true });
    position = mark.end;
  }
  if (position < to) {
    parts.push({ text: text.slice(position, to), marked: false });
  }
  if (from > 0) {
    parts.unshift({ text: "…", marked: false });
  }
  if (to < text.length) {
    parts.push({ text: "…", marked: false });
  }
  return parts;
};

const search = (posts: SearchPost[], sources: string[]): SearchResult[] => {
  const patterns = sources.map((source) => new RegExp(source, "giu"));
  const scored: (SearchResult & { score: number })[] = [];
  for (const post of posts) {
    const title = hitsOf(post.title, patterns);
    const body = hitsOf(post.text, patterns);
    const found = new Set([...title, ...body].map((hit) => hit.word));
    if (found.size < patterns.length) {
      continue;
    }
    scored.push({
      number: post.number,
      title: post.title,
      excerpt: excerptOf(post.text, body, patterns.length),
      score: scoreOf(title, body, patterns.length),
    });
  }
  // the sort is stable, so posts that score the same stay in the given order, newest first
  return scored
    .sort((a, b) => b.score - a.score)
    .map(({ score: _, ...result }) => result);
};

export interface SearchOptions {
  // an IME is still composing the query's last syllable
  composing?: boolean;
}

// a post matches when it contains every whitespace-separated word of the query, in its title or its text, anywhere and
// in any case
export const searchPosts = (
  posts: SearchPost[],
  query: string,
  { composing = false }: SearchOptions = {},
): SearchResult[] => {
  const words = query.normalize("NFC").split(/\s+/).filter(Boolean);
  if (words.length === 0) {
    return [];
  }
  const literal = search(posts, words.map(escapeRegExp));
  const last = words.at(-1)!;
  const loosened = composingPattern(last);
  if (!composing || loosened === escapeRegExp(last)) {
    return literal;
  }
  // the posts that match as typed come first, then those that match only what the last syllable may still become:
  // typing 시간 passes through 식, and a post with 식 somewhere shouldn't hide the ones with 시간
  const listed = new Set(literal.map((result) => result.number));
  const becoming = search(posts, [
    ...words.slice(0, -1).map(escapeRegExp),
    loosened,
  ]).filter((result) => !listed.has(result.number));
  return [...literal, ...becoming];
};
