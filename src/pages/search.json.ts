import type { APIRoute } from "astro";
import { getCollection } from "astro:content";
import { byNewest } from "../lib/posts";
import type { SearchPost } from "../lib/search";

// the search page matches queries against this in the browser (see lib/search.ts). Newest first, which is the order
// posts that match equally keep. NFC, so a query typed in either normalization finds the same text; each string is
// normalized before it is written, since normalizing the JSON could join an escape's last letter to a combining mark
export const GET: APIRoute = async () => {
  const posts: SearchPost[] = (await getCollection("posts"))
    .map((entry) => entry.data)
    .sort(byNewest)
    .map(({ number, title, searchText }) => ({
      number,
      title: title.normalize("NFC"),
      text: searchText.normalize("NFC"),
    }));

  return new Response(JSON.stringify(posts), {
    headers: { "Content-Type": "application/json" },
  });
};
