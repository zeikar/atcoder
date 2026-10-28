import type { Post } from "./posts";

export type TagSummary = {
  name: string;
  color: string;
  slug: string;
  count: number;
};

// lowercase, and turn characters that would end or escape a URL path segment into "-"; unicode, "+" and "&" stay readable.
// A slug comes from the name alone, so it never changes when other labels come and go. GitHub label names are unique
// regardless of case, so two labels only share a slug when they differ in the replaced characters ("help wanted" and
// "help-wanted"), which collectTags reports rather than giving either one a less readable URL.
export const tagSlug = (name: string): string => {
  const slug = name.toLowerCase().replace(/[\s/?#%\\]/g, "-");
  // URL resolution removes "." and ".." path segments, so /tags/./ and /tags/../ could never reach a tag page
  return slug === "." || slug === ".." ? slug.replaceAll(".", "-") : slug;
};

export const collectTags = (posts: Pick<Post, "tags">[]): TagSummary[] => {
  const bySlug = new Map<string, TagSummary>();

  for (const { tags } of posts) {
    for (const { name, color } of tags) {
      const slug = tagSlug(name);
      const tag = bySlug.get(slug);
      if (tag === undefined) {
        bySlug.set(slug, { name, color, slug, count: 1 });
      } else if (tag.name === name) {
        tag.count++;
      } else {
        // both labels would claim the same /tags/<slug>/ page
        throw new Error(
          `labels "${tag.name}" and "${name}" share the tag slug "${slug}"; rename one of them`,
        );
      }
    }
  }

  return [...bySlug.values()].sort(
    (a, b) => b.count - a.count || a.name.localeCompare(b.name),
  );
};

// how many of the most used tags a long tag list shows before folding the rest behind a toggle: with dozens of labels,
// the whole list would fill a phone's first screen before any post
export const SHOWN_TAGS = 12;

export const foldTags = <T extends { slug: string }>(
  tags: T[],
  currentSlug: string | undefined,
  limit = SHOWN_TAGS,
): { shown: T[]; folded: T[] } => {
  const shown = tags.slice(0, limit);
  const folded = tags.slice(limit);
  // the page's own tag stays in view
  const current = folded.findIndex((tag) => tag.slug === currentSlug);
  if (current !== -1) {
    shown.push(...folded.splice(current, 1));
  }
  // folding one tag would only swap its chip for the toggle's
  return folded.length < 2 ? { shown: tags, folded: [] } : { shown, folded };
};
