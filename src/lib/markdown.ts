import rehypeShiki from "@shikijs/rehype";
import type { Element, Nodes, Root } from "hast";
import { toString } from "hast-util-to-string";
import rehypeExternalLinks from "rehype-external-links";
import rehypeRaw from "rehype-raw";
import rehypeSanitize, {
  defaultSchema,
  type Options as SanitizeSchema,
} from "rehype-sanitize";
import rehypeSlug from "rehype-slug";
import rehypeStringify from "rehype-stringify";
import remarkBreaks from "remark-breaks";
import remarkGfm from "remark-gfm";
import remarkParse from "remark-parse";
import remarkRehype from "remark-rehype";
import { unified, type Plugin } from "unified";
import { EXIT, SKIP, visit } from "unist-util-visit";
import { nestHeadings, type Heading, type TocItem } from "./toc";

type PostMeta = {
  headings: Heading[];
  thumbnail: string | null;
  text: string;
  searchText: string;
};

declare module "vfile" {
  interface DataMap {
    postMeta: PostMeta;
    feedHtml: string;
    // the post's absolute address, for the links in feedHtml
    postUrl: string;
  }
}

export type RenderedMarkdown = {
  html: string;
  toc: TocItem[];
  excerpt: string;
  thumbnail: string | null;
  searchText: string;
  // the post for feeds (see rehypeCaptureFeedHtml)
  feedHtml: string;
};

const EXCERPT_LENGTH = 200;

// posts embed YouTube videos, but an iframe from any other origin is not trusted
// the id segment must end the path, or "/embed/../" would reach any other youtube.com page
const YOUTUBE_EMBED =
  /^https:\/\/www\.youtube(-nocookie)?\.com\/embed\/[\w-]+(?:[?#]|$)/;

const schema: SanitizeSchema = {
  ...defaultSchema,
  tagNames: [...(defaultSchema.tagNames ?? []), "iframe"],
  attributes: {
    ...defaultSchema.attributes,
    iframe: [
      "src",
      "width",
      "height",
      "title",
      "allow",
      "allowFullScreen",
      "frameBorder",
    ],
  },
};

const rehypeYoutubeOnly: Plugin<[], Root> = () => (tree) => {
  visit(tree, "element", (node, index, parent) => {
    if (node.tagName !== "iframe" || !parent || index === undefined) {
      return;
    }
    const { src } = node.properties;
    if (typeof src !== "string" || !YOUTUBE_EMBED.test(src)) {
      parent.children.splice(index, 1);
      return index;
    }
    // a named frame could be navigated to another origin by a link or form with target="<name>"
    delete node.properties.name;
  });
};

// sanitize prefixes every id with its default clobberPrefix but leaves hrefs alone, so in-page links need the same prefix to resolve;
// a link copied from GitHub's rendered page already carries it
const rehypePrefixFragmentLinks: Plugin<[], Root> = () => (tree) => {
  visit(tree, "element", (node) => {
    const { href } = node.properties;
    if (
      node.tagName === "a" &&
      typeof href === "string" &&
      href.startsWith("#") &&
      href.length > 1 &&
      !href.startsWith("#user-content-")
    ) {
      node.properties.href = "#user-content-" + href.slice(1);
    }
  });
};

const HEADING_TAG = /^h[1-6]$/;
// autolinks and links written as their own address show the URL itself as their text
const ADDRESS_TEXT = /^(?:[a-z][a-z\d+.-]*:\/\/|www\.)\S+$/i;

// elements whose text doesn't run on into the text next to them, as two table cells or the lines around a <br> don't
const SEPARATE_TEXT =
  /^(?:blockquote|br|dd|details|div|dt|h[1-6]|hr|li|ol|p|pre|section|summary|table|td|th|tr|ul)$/;

// the text of a node, like hast-util-to-string, minus the elements leftOut picks, with runs of whitespace collapsed
const textWithout = (
  node: Nodes,
  leftOut: (element: Element) => boolean,
): string => {
  const text = (node: Nodes): string => {
    if (node.type === "element" && leftOut(node)) {
      return "";
    }
    if ("children" in node) {
      const inner = node.children.map(text).join("");
      return node.type === "element" && SEPARATE_TEXT.test(node.tagName)
        ? ` ${inner} `
        : inner;
    }
    return node.type === "text" ? node.value : "";
  };
  return text(node).replace(/\s+/g, " ").trim();
};

// a bare address would only make every post from one site match words such as https or com
const isAddressLink = (element: Element): boolean =>
  element.tagName === "a" && ADDRESS_TEXT.test(toString(element).trim());

// what doesn't read as prose in a one-line preview: headings are the post's outline (the toc already shows them), and
// a code block or a bare address is noise
const leftOutOfExcerpt = (element: Element): boolean =>
  HEADING_TAG.test(element.tagName) ||
  element.tagName === "pre" ||
  isAddressLink(element);

// search reads headings, code and footnotes too, so a post is found by a section's name or a function it calls; left
// out are the footnote markers and back-links, whose numbers and arrows aren't words, and the footnotes section's
// visually hidden heading
const leftOutOfSearch = (element: Element): boolean => {
  const { className, dataFootnoteRef, dataFootnoteBackref } =
    element.properties;
  return (
    isAddressLink(element) ||
    dataFootnoteRef !== undefined ||
    dataFootnoteBackref !== undefined ||
    (Array.isArray(className) && className.includes("sr-only"))
  );
};

const rehypeCollectPostMeta: Plugin<[], Root> = () => (tree, file) => {
  // GFM appends the footnotes section to the root, and its visually hidden "Footnotes" heading belongs in neither the toc nor the excerpt;
  // rehype-raw turns the marker into an empty string, so test for presence
  const body: Root = {
    type: "root",
    children: tree.children.filter(
      (node) =>
        node.type !== "element" || node.properties.dataFootnotes === undefined,
    ),
  };
  const headings: Heading[] = [];
  let thumbnail: string | null = null;

  visit(body, "element", (node) => {
    const { id } = node.properties;
    const text = toString(node);
    // an empty heading such as a bare "##" line has nothing to show in the toc
    if (
      HEADING_TAG.test(node.tagName) &&
      typeof id === "string" &&
      text.trim() !== ""
    ) {
      headings.push({ id, depth: Number(node.tagName[1]), text });
    }
  });
  visit(tree, "element", (node) => {
    const { src } = node.properties;
    if (
      node.tagName === "img" &&
      thumbnail === null &&
      typeof src === "string"
    ) {
      thumbnail = src;
    }
  });

  file.data.postMeta = {
    headings,
    thumbnail,
    text: textWithout(body, leftOutOfExcerpt),
    searchText: textWithout(tree, leftOutOfSearch),
  };
};

// global.css lets an embed shrink to a narrow column, as YouTube's embed code sets width="560"; its width and height as an
// aspect ratio keep it the shape it was given. An embed without both as numbers, such as width="100%", keeps its height
const rehypeEmbedAspectRatio: Plugin<[], Root> = () => (tree) => {
  visit(tree, "element", (node) => {
    const width = Number(node.properties.width);
    const height = Number(node.properties.height);
    if (
      node.tagName === "iframe" &&
      Number.isFinite(width) &&
      Number.isFinite(height) &&
      width > 0 &&
      height > 0
    ) {
      node.properties.style = `aspect-ratio: ${width} / ${height}; height: auto`;
    }
  });
};

const rehypeLazyImages: Plugin<[], Root> = () => (tree) => {
  visit(tree, "element", (node) => {
    if (node.tagName === "img") {
      node.properties.loading = "lazy";
    }
  });
};

const holdsLink = (node: Element): boolean => {
  let found = false;
  visit(node, "element", (child) => {
    if (child.tagName === "a") {
      found = true;
      return EXIT;
    }
  });
  return found;
};

// a heading's text links to the heading, so a section can be shared (global.css shows a "#" on hover); being the text
// itself, the link adds nothing to the heading's accessible name. Skipped: an empty heading, the footnotes section's
// visually hidden one, where a link would still take a Tab stop, one in or around a link, since links can't nest, and
// one in a summary, where a click on a link doesn't open the details
const rehypeHeadingLinks: Plugin<[], Root> = () => (tree) => {
  visit(tree, "element", (node) => {
    if (node.tagName === "a" || node.tagName === "summary") {
      return SKIP;
    }
    const { id, className } = node.properties;
    if (
      !HEADING_TAG.test(node.tagName) ||
      typeof id !== "string" ||
      toString(node).trim() === "" ||
      (Array.isArray(className) && className.includes("sr-only")) ||
      holdsLink(node)
    ) {
      return;
    }
    node.children = [
      {
        type: "element",
        tagName: "a",
        properties: { href: `#${id}`, className: ["heading-link"] },
        children: node.children,
      },
    ];
  });
};

// CopyCode.astro puts its button in this box rather than in the pre, where it would scroll away with a long line; a
// block in a link the post wrote around it gets no box, since a click on the button would follow the link
const rehypeWrapCodeBlocks: Plugin<[], Root> = () => (tree) => {
  visit(tree, "element", (node, index, parent) => {
    if (node.tagName === "a") {
      return SKIP;
    }
    if (node.tagName !== "pre" || !parent || index === undefined) {
      return;
    }
    parent.children[index] = {
      type: "element",
      tagName: "div",
      properties: { className: ["code-block"] },
      children: [node],
    };
    // past the new box, whose pre would otherwise be wrapped again
    return [SKIP, index + 1];
  });
};

// shiki language ids are lowercase, but posts use fences like ```Python
const rehypeLowercaseLanguage: Plugin<[], Root> = () => (tree) => {
  visit(tree, "element", (node) => {
    const { className } = node.properties;
    if (node.tagName === "code" && Array.isArray(className)) {
      node.properties.className = className.map((name) =>
        typeof name === "string" && name.startsWith("language-")
          ? name.toLowerCase()
          : name,
      );
    }
  });
};

const feedStringifier = unified().use(rehypeStringify);

// the post as a feed carries it: sanitized, and without what the rest of the pipeline adds for the site's own page, such
// as heading links and highlighted code, whose inline styles make up three quarters of a post's HTML and which a feed
// reader mostly drops. A reader shows the post away from its page, so in-page links (#user-content-…) get the post's
// address; they change on a copy, since the page keeps them as they are
const rehypeCaptureFeedHtml: Plugin<[], Root> = () => (tree, file) => {
  const feed = structuredClone(tree);
  const { postUrl } = file.data;
  if (postUrl !== undefined) {
    visit(feed, "element", (node) => {
      const { href } = node.properties;
      if (
        node.tagName === "a" &&
        typeof href === "string" &&
        href.startsWith("#")
      ) {
        node.properties.href = postUrl + href;
      }
    });
  }
  file.data.feedHtml = feedStringifier.stringify(feed);
};

const processor = unified()
  .use(remarkParse)
  .use(remarkGfm)
  .use(remarkBreaks)
  // sanitize adds the clobber prefix to every id, so footnote ids must start bare or they get it twice
  .use(remarkRehype, { allowDangerousHtml: true, clobberPrefix: "" })
  .use(rehypeRaw)
  .use(rehypeSlug)
  // issue bodies are untrusted and this is their only sanitizer: plugins after it must add only markup they generate
  .use(rehypeSanitize, schema)
  .use(rehypeYoutubeOnly)
  .use(rehypePrefixFragmentLinks)
  .use(rehypeCollectPostMeta)
  .use(rehypeCaptureFeedHtml)
  .use(rehypeEmbedAspectRatio)
  .use(rehypeLazyImages)
  .use(rehypeHeadingLinks)
  .use(rehypeExternalLinks, {
    target: "_blank",
    rel: ["noopener", "noreferrer"],
  })
  .use(rehypeLowercaseLanguage)
  .use(rehypeShiki, {
    // GitHub's current themes: the older github-light and github-dark fall below 4.5:1 on the code background
    // (--color-surface in global.css), such as the light keyword red at 4.2:1 and the dark comment gray at 3.6:1
    themes: { light: "github-light-default", dark: "github-dark-default" },
    // the light theme's comment gray is still 4.2:1 there, so comments take the site's muted text color, at 5.3:1
    colorReplacements: { "github-light-default": { "#6e7781": "#5f6672" } },
    fallbackLanguage: "text",
  })
  .use(rehypeWrapCodeBlocks)
  .use(rehypeStringify);

export const renderMarkdown = async (
  markdown: string,
  { postUrl }: { postUrl?: string } = {},
): Promise<RenderedMarkdown> => {
  const file = await processor.process({ value: markdown, data: { postUrl } });
  // rehypeCollectPostMeta runs on every file
  const { headings, thumbnail, text, searchText } = file.data.postMeta!;
  // cut by code point so an emoji at the boundary is not left as a lone surrogate
  const characters = Array.from(text);

  return {
    html: String(file),
    toc: nestHeadings(headings),
    excerpt:
      characters.length > EXCERPT_LENGTH
        ? characters.slice(0, EXCERPT_LENGTH).join("") + "…"
        : text,
    thumbnail,
    searchText,
    // rehypeCaptureFeedHtml runs on every file too
    feedHtml: file.data.feedHtml!,
  };
};
