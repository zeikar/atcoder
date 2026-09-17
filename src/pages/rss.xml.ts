import rss from "@astrojs/rss";
import type { APIRoute } from "astro";
import { getCollection } from "astro:content";
import { siteConfig } from "../lib/config";
import { postPath, withBase } from "../lib/links";
import { byNewest } from "../lib/posts";
import { siteDescription, siteUrl, xmlSafe } from "../lib/seo";
import { getSite } from "../lib/site";

// only the newest posts, as jekyll-feed and eleventy-base-blog keep only their newest 10: every post in full would make a
// site of long posts a feed of megabytes, fetched again on each change. Older posts aren't kept as excerpts instead, since
// a reader that refreshes stored items would replace a full post it saved with the excerpt
const FEED_POSTS = 20;

export const GET: APIRoute = async ({ site }) => {
  const [posts, siteData] = await Promise.all([
    getCollection("posts"),
    getSite(),
  ]);
  const { websiteTitle, language } = siteConfig;

  return rss({
    title: websiteTitle,
    // RSS requires a channel description
    description: siteDescription(siteData) ?? websiteTitle,
    site: siteUrl(import.meta.env.BASE_URL, "/", site),
    items: posts
      .map((entry) => entry.data)
      .sort(byNewest)
      .slice(0, FEED_POSTS)
      .map((post) => ({
        title: xmlSafe(post.title),
        link: withBase(import.meta.env.BASE_URL, postPath(post.number)),
        pubDate: new Date(post.createdAt),
        description: xmlSafe(post.excerpt),
        // the whole post, so a reader can read it without leaving the feed; an empty body is left out, since a feed whose
        // items all lack content doesn't declare the content namespace their empty elements would use
        content: xmlSafe(post.feedHtml) || undefined,
        categories: post.tags.map((tag) => xmlSafe(tag.name)),
      })),
    // parseSiteConfig only accepts a language tag, so it is safe to write into the XML as is
    customData: `<language>${language}</language>`,
  });
};
