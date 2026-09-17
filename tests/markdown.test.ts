// @vitest-environment jsdom
// @ts-expect-error: the project has no @types/node (see src/lib/github.ts)
import { readFileSync } from "node:fs";
import { beforeAll, describe, expect, it } from "vitest";
import { renderMarkdown } from "../src/lib/markdown";
import { nestHeadings, type TocItem } from "../src/lib/toc";

// the first render loads every Shiki grammar (seconds on a busy CI runner); pay that once here,
// with a generous timeout, instead of inside whichever test happens to run first
beforeAll(async () => {
  await renderMarkdown("```js\nx\n```");
}, 60_000);

const render = async (markdown: string): Promise<Document> => {
  const { html } = await renderMarkdown(markdown);
  return new DOMParser().parseFromString(html, "text/html");
};

const YOUTUBE_EMBED =
  /^https:\/\/www\.youtube(-nocookie)?\.com\/embed\/[\w-]+(?:[?#]|$)/;

// the "text" fallback paints a line in one color, so real highlighting shows up as several
const tokenStyles = (doc: Document): Set<string | null> =>
  new Set(
    Array.from(doc.querySelectorAll("pre.shiki span[style]"), (span) =>
      span.getAttribute("style"),
    ),
  );

// the code background in each theme, read from the tokens in global.css
const SURFACE = (() => {
  // read from disk, since vitest hands CSS imports over empty; it runs from the project root
  const css: string = readFileSync("src/styles/global.css", "utf8").replace(
    /\/\*[\s\S]*?\*\//g,
    "",
  );
  const [light, dark] = Array.from(
    css.matchAll(/--color-surface:\s*(#[0-9a-f]{6})/gi),
    (match) => match[1],
  );
  return { light, dark };
})();

// WCAG contrast ratio of two sRGB hex colors
const contrast = (a: string, b: string): number => {
  const luminance = (hex: string) => {
    const [r, g, bl] = [1, 3, 5].map((i) => {
      const c = parseInt(hex.slice(i, i + 2), 16) / 255;
      return c <= 0.03928 ? c / 12.92 : ((c + 0.055) / 1.055) ** 2.4;
    });
    return 0.2126 * r + 0.7152 * g + 0.0722 * bl;
  };
  const [high, low] = [luminance(a), luminance(b)].sort((x, y) => y - x);
  return (high + 0.05) / (low + 0.05);
};

const flattenToc = (items: TocItem[]): TocItem[] =>
  items.flatMap((item) => [item, ...flattenToc(item.children)]);

describe("renderMarkdown html", () => {
  // anyone who can get markdown into a post must not be able to run script on the site
  it.each([
    ["an inline event handler", '<img src="x" onerror="alert(1)">'],
    ["a script tag", "<script>alert(1)</script>"],
    ["an svg onload", "<svg onload=alert(1)>"],
    ["an iframe", '<iframe src="https://evil.example"></iframe>'],
    [
      "an iframe on a YouTube lookalike host",
      '<iframe src="https://www.youtube.com.evil.example/embed/x"></iframe>',
    ],
    [
      "an iframe whose path climbs out of /embed/",
      '<iframe src="https://www.youtube.com/embed/../redirect?q=https://evil.example"></iframe>',
    ],
    [
      "a frame name that links could use to navigate an embed elsewhere",
      '<iframe name="yt" src="https://www.youtube.com/embed/x"></iframe><a href="https://evil.example" target="yt">x</a>',
    ],
    [
      "srcdoc on an allowed embed",
      '<iframe src="https://www.youtube.com/embed/x" srcdoc="<script>alert(1)</script>"></iframe>',
    ],
    ["a javascript: markdown link", "[click](javascript:alert(1))"],
    ["a javascript: html link", '<a href="javascript:alert(1)">click</a>'],
    [
      "a quote breaking out of the link renderer",
      '[x](https://a.example/"onmouseover="alert(1))',
    ],
    ["a code fence meta string", "```js onclick=alert(1)\nx\n```"],
  ])("removes %s", async (_, markdown) => {
    const doc = await render(markdown);

    expect(doc.querySelector("script")).toBeNull();
    for (const frame of Array.from(doc.querySelectorAll("iframe"))) {
      expect(frame.getAttribute("src")).toMatch(YOUTUBE_EMBED);
      expect(frame.hasAttribute("srcdoc")).toBe(false);
      expect(frame.hasAttribute("name")).toBe(false);
    }
    for (const element of Array.from(doc.querySelectorAll("*"))) {
      for (const { name, value } of Array.from(element.attributes)) {
        expect(name).not.toMatch(/^on/i);
        expect(value).not.toMatch(/^\s*javascript:/i);
      }
    }
  });

  it("keeps YouTube embeds, which existing posts use", async () => {
    const doc = await render(
      '<iframe width="560" height="315" src="https://www.youtube.com/embed/GTJr8OvyEVQ" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; encrypted-media" allowfullscreen></iframe>',
    );
    const frame = doc.querySelector("iframe");

    expect(frame?.getAttribute("src")).toBe(
      "https://www.youtube.com/embed/GTJr8OvyEVQ",
    );
    expect(frame?.hasAttribute("allowfullscreen")).toBe(true);
  });

  it("opens external links in a new tab", async () => {
    const link = (await render("[site](https://example.com)")).querySelector(
      "a",
    );

    expect(link?.getAttribute("href")).toBe("https://example.com");
    expect(link?.getAttribute("target")).toBe("_blank");
  });

  it("keeps images and highlights code", async () => {
    const doc = await render(
      "![alt](https://example.com/a.png)\n\n```js\nconst a = 1;\n```",
    );

    expect(doc.querySelector("img")?.getAttribute("src")).toBe(
      "https://example.com/a.png",
    );
    expect(doc.querySelector("pre.shiki")).not.toBeNull();
    expect(tokenStyles(doc).size).toBeGreaterThan(1);
  });

  it("puts each code block in a box for its copy button, leaving the pre as it was", async () => {
    const doc = await render(
      "```js\nconst a = 1;\n```\n\n<pre>no code element</pre>\n\n- ```\n  in a list\n  ```",
    );
    const blocks = Array.from(doc.querySelectorAll("pre"));

    expect(blocks).toHaveLength(3);
    for (const pre of blocks) {
      expect(pre.parentElement?.className).toBe("code-block");
      expect(pre.parentElement?.children).toHaveLength(1);
    }
    // Shiki's tabindex, which lets a keyboard scroll a long line
    expect(blocks[0].getAttribute("tabindex")).toBe("0");
    expect(doc.querySelectorAll(".code-block .code-block")).toHaveLength(0);
  });

  it("gives no box to a code block in a link the post wrote around it, where a copy button would follow the link", async () => {
    const doc = await render(
      '<a href="https://example.com">\n\n```js\nconst a = 1;\n```\n\n</a>',
    );

    expect(doc.querySelector("a pre")).not.toBeNull();
    expect(doc.querySelector(".code-block")).toBeNull();
  });

  it("lazy-loads images, whatever the post asks for", async () => {
    const doc = await render(
      '![a](https://example.com/a.png)\n\n<img src="https://example.com/b.png" loading="eager">',
    );
    const images = Array.from(doc.querySelectorAll("img"));

    expect(images).toHaveLength(2);
    for (const image of images) {
      expect(image.getAttribute("loading")).toBe("lazy");
    }
  });

  it("keeps every code color at 4.5:1 against the code background, in both themes", async () => {
    const { html } = await renderMarkdown(
      [
        "```python",
        "@cache",
        "def solve(self, nums: List[int], k=3) -> int:",
        '    """docstring"""',
        "    # a comment",
        "    return len(nums) + 1 if nums else f'{k}'",
        "```",
        "```cpp",
        "#include <vector>",
        "template <typename T> T add(const T& a) { return a << 2; } // note",
        "```",
        "```ts",
        "const re = /a+b/g; export class A extends B { private x?: number = 0x1f; }",
        "```",
        "```diff",
        "- removed",
        "+ added",
        "```",
      ].join("\n"),
    );
    const doc = new DOMParser().parseFromString(html, "text/html");
    const styles = Array.from(
      doc.querySelectorAll("pre.shiki span[style]"),
      (span) => span.getAttribute("style")!,
    );
    const light = styles.flatMap(
      (style) => style.match(/(?:^|;)color:(#[0-9a-f]{6})/i)?.[1] ?? [],
    );
    const dark = styles.flatMap(
      (style) => style.match(/--shiki-dark:(#[0-9a-f]{6})/i)?.[1] ?? [],
    );

    expect(new Set(light).size).toBeGreaterThan(5);
    expect(new Set(dark).size).toBeGreaterThan(5);
    for (const color of new Set(light)) {
      expect([color, contrast(color, SURFACE.light)]).toEqual([
        color,
        expect.toSatisfy((ratio: number) => ratio >= 4.5),
      ]);
    }
    for (const color of new Set(dark)) {
      expect([color, contrast(color, SURFACE.dark)]).toEqual([
        color,
        expect.toSatisfy((ratio: number) => ratio >= 4.5),
      ]);
    }
  });

  it("highlights a capitalized language name", async () => {
    const doc = await render("```Python\ndef f():\n    return 1\n```");

    expect(tokenStyles(doc).size).toBeGreaterThan(1);
  });

  it("renders a fence whose info string is not a language", async () => {
    const doc = await render("```DP\ndp[i] = dp[i - 1] + 1\n```");

    expect(doc.querySelector("pre code")?.textContent).toContain(
      "dp[i] = dp[i - 1] + 1",
    );
  });
});

describe("renderMarkdown links and ids", () => {
  it("builds a nested table of contents whose ids exist in the html", async () => {
    // "title", "body" and "links" are document properties, so bare ids would clobber them
    const { html, toc } = await renderMarkdown(
      "# Title\n\n## Body\n\n## Links\n\n### 소개",
    );
    const doc = new DOMParser().parseFromString(html, "text/html");
    const items = flattenToc(toc);

    expect(items).toHaveLength(4);
    for (const { id } of items) {
      expect(id).toMatch(/^user-content-/);
      expect(doc.getElementById(id)).not.toBeNull();
    }
    const shape = (items: TocItem[]): unknown[] =>
      items.map(({ text, depth, children }) => [text, depth, shape(children)]);
    expect(shape(toc)).toEqual([
      [
        "Title",
        1,
        [
          ["Body", 2, []],
          ["Links", 2, [["소개", 3, []]]],
        ],
      ],
    ]);
  });

  it("makes each heading's text a link to the heading, leaving its text as it was", async () => {
    const { html, toc } = await renderMarkdown("# Title\n\n## `code` Body");
    const doc = new DOMParser().parseFromString(html, "text/html");
    const headings = Array.from(doc.querySelectorAll("h1, h2"));

    expect(headings).toHaveLength(2);
    for (const heading of headings) {
      const link = heading.firstElementChild;
      expect(heading.children).toHaveLength(1);
      expect(link?.tagName).toBe("A");
      expect(link?.getAttribute("href")).toBe(`#${heading.id}`);
    }
    expect(headings.map((heading) => heading.textContent)).toEqual([
      "Title",
      "code Body",
    ]);
    expect(flattenToc(toc).map(({ text }) => text)).toEqual([
      "Title",
      "code Body",
    ]);
  });

  it("links no heading that has nothing to show or already holds a link", async () => {
    const doc = await render(
      "##\n\n## [Docs](https://example.com)\n\nText[^1]\n\n[^1]: Note",
    );

    // the empty heading, the one with a link, since links can't nest, and the footnotes section's visually hidden
    // heading, where a link would still take a Tab stop
    expect(doc.querySelectorAll("h2")).toHaveLength(3);
    expect(doc.querySelectorAll("h2 a")).toHaveLength(1);
    expect(doc.querySelector("h2 a")?.getAttribute("href")).toBe(
      "https://example.com",
    );
  });

  it("links no heading in a summary, which a click must still open, or in a link the post wrote around it", async () => {
    const doc = await render(
      '<details><summary><h3>Show solution</h3></summary>\n\nx\n\n</details>\n\n<a href="https://example.com">\n\n## Title\n\n</a>',
    );

    expect(doc.querySelectorAll("h2, h3")).toHaveLength(2);
    expect(doc.querySelectorAll(".heading-link")).toHaveLength(0);
  });

  it("leaves empty headings out of the table of contents", async () => {
    const { toc } = await renderMarkdown("##\n\n# Real");

    expect(flattenToc(toc).map(({ text }) => text)).toEqual(["Real"]);
  });

  it("points in-page links at the prefixed ids", async () => {
    const doc = await render(
      "[jump](#body)\n\n[x](https://a.example/#body)\n\n## Body",
    );
    const [jump, external] = Array.from(doc.querySelectorAll("a"));

    expect(jump.getAttribute("href")).toBe("#user-content-body");
    expect(doc.getElementById("user-content-body")).not.toBeNull();
    expect(external.getAttribute("href")).toBe("https://a.example/#body");
  });

  it("keeps in-page links already written with the prefix, as GitHub resolves them", async () => {
    const doc = await render("[jump](#user-content-body)\n\n## Body");

    expect(doc.querySelector("a")!.getAttribute("href")).toBe(
      "#user-content-body",
    );
  });

  it("links footnote references and back-references to existing ids", async () => {
    const { html, toc, excerpt } = await renderMarkdown(
      "Text[^1]\n\n[^1]: Note",
    );
    const doc = new DOMParser().parseFromString(html, "text/html");
    const links = Array.from(doc.querySelectorAll('a[href^="#"]'));

    // the footnotes section's heading is visually hidden, so it is not part of the post's outline or summary
    expect(toc).toEqual([]);
    expect(excerpt).not.toContain("Footnotes");
    expect(excerpt).not.toContain("Note");
    expect(excerpt).not.toContain("↩");

    expect(links.length).toBeGreaterThanOrEqual(2);
    for (const link of links) {
      const id = link.getAttribute("href")!.slice(1);
      expect(doc.getElementById(id)).not.toBeNull();
    }
    for (const element of Array.from(doc.querySelectorAll("[id]"))) {
      expect(element.id).not.toContain("user-content-user-content-");
    }
  });
});

describe("renderMarkdown search text", () => {
  it("keeps headings and code, and leaves out bare addresses and the footnotes heading", async () => {
    const { searchText } = await renderMarkdown(
      "# Problem link\n\nhttps://leetcode.com/problems/two-sum\n\n[the docs](https://example.com) say\n\n```python\nimport heapq\n```\n\nText[^1]\n\n[^1]: Note",
    );

    expect(searchText).toContain("Problem link");
    expect(searchText).toContain("the docs say");
    expect(searchText).toContain("import heapq");
    expect(searchText).not.toContain("leetcode.com");
    expect(searchText).toContain("Note");
    expect(searchText).not.toContain("Footnotes");
    expect(searchText).not.toContain("↩");
    expect(searchText).not.toMatch(/\s{2}/);
  });

  it("keeps words apart where elements separate them", async () => {
    const { searchText, excerpt } = await renderMarkdown(
      "<p>줄1<br>줄2</p>\n\n| 셀1 | 셀2 |\n| --- | --- |\n| 셀3 | 셀4 |\n\n<details><summary>힌트</summary>숨긴 내용</details>",
    );

    for (const words of ["줄1 줄2", "셀1 셀2", "셀3 셀4", "힌트 숨긴"]) {
      expect(searchText).toContain(words);
    }
    expect(excerpt).toContain("줄1 줄2");
  });
});

describe("renderMarkdown excerpt and thumbnail", () => {
  it("returns decoded text without markup, so it must never be rendered as HTML", async () => {
    const { excerpt } = await renderMarkdown(
      "**bold** <img src=x onerror=alert(1)> 1 &lt; 2",
    );

    expect(excerpt).toContain("bold");
    expect(excerpt).toContain("1 < 2");
    expect(excerpt).not.toContain("<img");
  });

  it("truncates long text with an ellipsis", async () => {
    expect((await renderMarkdown("a".repeat(300))).excerpt).toBe(
      "a".repeat(200) + "…",
    );
    expect((await renderMarkdown("short")).excerpt).toBe("short");
  });

  it("does not split an emoji at the cut", async () => {
    const { excerpt } = await renderMarkdown("a".repeat(199) + "😀 more text");

    expect(excerpt).toMatch(/^a{199}😀…$/u);
  });

  it("starts at the prose of a post laid out as headings and a link", async () => {
    const { excerpt, toc } = await renderMarkdown(
      "# Problem link\nhttps://leetcode.com/problems/two-sum/\n\n# Problem Summary\n배열에서 두 수의 합이 target이 되는 문제.\n\n## Solution\n해시맵을 쓴다.",
    );

    expect(excerpt).toBe(
      "배열에서 두 수의 합이 target이 되는 문제. 해시맵을 쓴다.",
    );
    // headings are dropped from the excerpt only; the toc keeps them
    expect(flattenToc(toc).map(({ text }) => text)).toEqual([
      "Problem link",
      "Problem Summary",
      "Solution",
    ]);
  });

  it.each([
    ["an angle-bracket autolink", "see <https://e.com/a> here"],
    ["a www literal", "see www.e.com here"],
    [
      "a link written with its own address",
      "see [https://e.com/a](https://e.com/a) here",
    ],
    [
      "an address the href percent-encodes",
      "see https://ko.wikipedia.org/wiki/동적_계획법 here",
    ],
    [
      "a raw html link whose address sits on its own line",
      'see\n\n<a href="https://e.com/a">\nhttps://e.com/a\n</a>\n\nhere',
    ],
    ["a heading nested in a blockquote", "> # Title\n\nsee here"],
    ["a code block", "see\n\n```python\nclass Solution: pass\n```\n\nhere"],
  ])("leaves out %s", async (_, markdown) => {
    expect((await renderMarkdown(markdown)).excerpt).toBe("see here");
  });

  it("keeps an address written as inline code, which is not a link", async () => {
    expect((await renderMarkdown("see `https://e.com/a` here")).excerpt).toBe(
      "see https://e.com/a here",
    );
  });

  it("keeps the words of a link that names its target, so the sentence still reads", async () => {
    const { excerpt } = await renderMarkdown(
      "[공식 문서](https://docs.python.org/3/)를 참고했다.",
    );

    expect(excerpt).toBe("공식 문서를 참고했다.");
  });

  it("handles an empty body", async () => {
    const { excerpt, thumbnail, toc } = await renderMarkdown("");

    expect(excerpt).toBe("");
    expect(thumbnail).toBeNull();
    expect(toc).toEqual([]);
  });

  it.each([
    ["a markdown image", "![](https://e.com/a.png)", "https://e.com/a.png"],
    ["an html image", '<img src="https://e.com/b.png">', "https://e.com/b.png"],
  ])("uses %s as the thumbnail", async (_, markdown, expected) => {
    expect((await renderMarkdown(markdown)).thumbnail).toBe(expected);
  });
});

describe("nestHeadings", () => {
  it("nests a heading under the closest earlier heading of smaller depth", () => {
    const toc = nestHeadings([
      { id: "a", text: "A", depth: 3 },
      { id: "b", text: "B", depth: 1 },
      { id: "c", text: "C", depth: 3 },
      { id: "d", text: "D", depth: 2 },
    ]);

    expect(toc).toEqual([
      { id: "a", text: "A", depth: 3, children: [] },
      {
        id: "b",
        text: "B",
        depth: 1,
        children: [
          { id: "c", text: "C", depth: 3, children: [] },
          { id: "d", text: "D", depth: 2, children: [] },
        ],
      },
    ]);
  });
});
