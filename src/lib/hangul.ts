// While a Korean IME composes a word, the search box holds its last syllable half-built: typing 배열 shows ㅂ, 배, 뱅,
// 배여 and then 배열. composingPattern turns that syllable into every syllable it can still become, as a RegExp source,
// so the posts don't disappear from the results between keystrokes.

const SYLLABLE_BASE = 0xac00;
const SYLLABLE_COUNT = 11172;
// the initial consonants, in syllable order, as the compatibility jamo an IME shows on its own
const INITIALS = "ㄱㄲㄴㄷㄸㄹㅁㅂㅃㅅㅆㅇㅈㅉㅊㅋㅌㅍㅎ";
// a vowel that can still grow into a compound vowel: ㅗ into ㅘ ㅙ ㅚ, ㅜ into ㅝ ㅞ ㅟ, ㅡ into ㅢ
const VOWEL_GROWTH: Record<number, number[]> = {
  8: [9, 10, 11],
  13: [14, 15, 16],
  18: [19],
};
// a final that can still grow into a compound final: ㄱ into ㄳ, ㄴ into ㄵ ㄶ, ㄹ into ㄺ to ㅀ, ㅂ into ㅄ
const FINAL_GROWTH: Record<number, number[]> = {
  1: [3],
  4: [5, 6],
  8: [9, 10, 11, 12, 13, 14, 15],
  17: [18],
};
// when a vowel follows, a final moves on as the next syllable's initial; a compound final leaves its first half behind.
// [final left behind, initial moved on]
const FINAL_MOVES: Record<number, [number, number]> = {
  1: [0, 0],
  2: [0, 1],
  3: [1, 9],
  4: [0, 2],
  5: [4, 12],
  6: [4, 18],
  7: [0, 3],
  8: [0, 5],
  9: [8, 0],
  10: [8, 6],
  11: [8, 7],
  12: [8, 9],
  13: [8, 16],
  14: [8, 17],
  15: [8, 18],
  16: [0, 6],
  17: [0, 7],
  18: [17, 9],
  19: [0, 9],
  20: [0, 10],
  21: [0, 11],
  22: [0, 12],
  23: [0, 14],
  24: [0, 15],
  25: [0, 16],
  26: [0, 17],
  27: [0, 18],
};

const syllable = (initial: number, vowel: number, final = 0): string =>
  String.fromCharCode(SYLLABLE_BASE + (initial * 21 + vowel) * 28 + final);

// every syllable with this initial, as a character class range
const withInitial = (initial: number): string =>
  `${syllable(initial, 0)}-${syllable(initial, 20, 27)}`;

export const escapeRegExp = (text: string): string =>
  text.replace(/[.*+?^${}()|[\]\\]/g, "\\$&");

export const composingPattern = (word: string): string => {
  const last = word.at(-1) ?? "";
  const head = escapeRegExp(word.slice(0, -1));
  const initialAlone = INITIALS.indexOf(last);
  if (initialAlone !== -1) {
    return `${head}[${withInitial(initialAlone)}]`;
  }
  const code = last.charCodeAt(0) - SYLLABLE_BASE;
  if (!(code >= 0 && code < SYLLABLE_COUNT)) {
    return escapeRegExp(word);
  }
  const initial = Math.floor(code / 588);
  const vowel = Math.floor(code / 28) % 21;
  const final = code % 28;
  if (final === 0) {
    const ranges = [vowel, ...(VOWEL_GROWTH[vowel] ?? [])].map(
      (grown) => `${syllable(initial, grown)}-${syllable(initial, grown, 27)}`,
    );
    return `${head}[${ranges.join("")}]`;
  }
  const kept = [final, ...(FINAL_GROWTH[final] ?? [])].map((grown) =>
    syllable(initial, vowel, grown),
  );
  const [leftBehind, movedOn] = FINAL_MOVES[final];
  return `${head}(?:[${kept.join("")}]|${syllable(initial, vowel, leftBehind)}[${withInitial(movedOn)}])`;
};
