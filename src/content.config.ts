import { defineCollection } from "astro:content";
import { z } from "astro/zod";
import { siteConfig } from "./lib/config";
import {
  fetchDiscussionCategories,
  fetchDiscussions,
  fetchIssues,
  fetchPinnedNumbers,
  fetchProfile,
  fetchRepositoryDescription,
} from "./lib/github";
import { postPath } from "./lib/links";
import { renderMarkdown } from "./lib/markdown";
import { isTrustedPost, toPost } from "./lib/posts";
import { siteUrl } from "./lib/seo";
import { tagSlug } from "./lib/tags";
import type { TocItem } from "./lib/toc";

const { repoOwner, repoName, source } = siteConfig;

// both collections need the categories in discussions mode, and Astro starts their loaders together, so they share one
// request; it is dropped once settled, so a dev server's later content sync sees a renamed or deleted category
let categoriesRequest: ReturnType<typeof fetchDiscussionCategories> | undefined;
const discussionCategories = () =>
  (categoriesRequest ??= fetchDiscussionCategories(
    repoOwner,
    repoName,
    siteConfig.discussionCategories,
  ).finally(() => {
    categoriesRequest = undefined;
  }));

const fetchPostNodes = async () => {
  if (source === "issues") {
    return fetchIssues(repoOwner, repoName);
  }
  // the categoryId argument takes one category, so each is listed on its own
  const { categoryIds } = await discussionCategories();
  const lists = await Promise.all(
    categoryIds.map((id) => fetchDiscussions(repoOwner, repoName, id)),
  );
  return lists.flat();
};

const tocItem: z.ZodType<TocItem> = z.lazy(() =>
  z.object({
    id: z.string(),
    text: z.string(),
    depth: z.number(),
    children: z.array(tocItem),
  }),
);

const posts = defineCollection({
  loader: {
    name: "github-posts",
    load: async ({ store, logger, parseData }) => {
      const [nodes, pinned] = await Promise.all([
        fetchPostNodes(),
        fetchPinnedNumbers(repoOwner, repoName, source),
      ]);
      const trusted = nodes
        .filter((node) => isTrustedPost(node, siteConfig))
        .map(toPost);

      store.clear();
      for (const post of trusted) {
        const { html, toc, excerpt, thumbnail, searchText, feedHtml } =
          await renderMarkdown(post.body, {
            postUrl: siteUrl(
              import.meta.env.BASE_URL,
              postPath(post.number),
              import.meta.env.SITE ? new URL(import.meta.env.SITE) : undefined,
            ),
          }).catch((error: unknown) => {
            throw new Error(`rendering ${post.url} failed`, { cause: error });
          });
        const id = String(post.number);
        const data = await parseData({
          id,
          data: {
            number: post.number,
            title: post.title,
            createdAt: post.createdAt,
            updatedAt: post.updatedAt,
            url: post.url,
            commentCount: post.commentCount,
            pinned: pinned.has(post.number),
            category: post.category && {
              id: post.category.id,
              name: post.category.name,
            },
            tags: post.tags.map((tag) => ({ ...tag, slug: tagSlug(tag.name) })),
            excerpt,
            thumbnail,
            toc,
            searchText,
            feedHtml,
          },
        });
        store.set({ id, data, rendered: { html } });
      }

      logger.info(
        `Loaded ${trusted.length} posts from ${repoOwner}/${repoName} ${source}`,
      );
    },
  },
  schema: z.object({
    number: z.number(),
    title: z.string(),
    createdAt: z.string(),
    updatedAt: z.string(),
    url: z.string(),
    commentCount: z.number(),
    // pinned to the top of the repository's issue or discussion list on GitHub
    pinned: z.boolean(),
    // the discussion's category for giscus; null in issues mode
    category: z.object({ id: z.string(), name: z.string() }).nullable(),
    tags: z.array(
      z.object({
        name: z.string(),
        // label colors are interpolated into style attributes, so only a hex value may pass
        color: z.string().regex(/^[0-9a-f]{6}$/i),
        slug: z.string(),
      }),
    ),
    excerpt: z.string(),
    thumbnail: z.string().nullable(),
    toc: z.array(tocItem),
    // the post's plain text for search.json
    searchText: z.string(),
    // the post's HTML for rss.xml
    feedHtml: z.string(),
  }),
});

const site = defineCollection({
  loader: async () => {
    const [profile, description, discussion] = await Promise.all([
      fetchProfile(repoOwner),
      fetchRepositoryDescription(repoOwner, repoName),
      source === "discussions" ? discussionCategories() : null,
    ]);
    return [
      {
        id: "site",
        profile,
        description,
        // giscus needs the repository id, and only discussions mode uses giscus
        repoId: discussion?.repoId ?? null,
      },
    ];
  },
  schema: z.object({
    profile: z.object({
      login: z.string(),
      name: z.string().nullable(),
      bio: z.string().nullable(),
      avatarUrl: z.string(),
      url: z.string(),
      followers: z.object({ totalCount: z.number() }),
      following: z.object({ totalCount: z.number() }),
    }),
    description: z.string().nullable(),
    repoId: z.string().nullable(),
  }),
});

export const collections = { posts, site };
