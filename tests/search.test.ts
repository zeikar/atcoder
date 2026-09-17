import { describe, expect, it } from "vitest";
import { searchPosts, type SearchPost } from "../src/lib/search";

const post = (number: number, title: string, text: string): SearchPost => ({
  number,
  title,
  text,
});

const numbers = (posts: SearchPost[], query: string): number[] =>
  searchPosts(posts, query).map((result) => result.number);

const marked = (posts: SearchPost[], query: string): string[] =>
  searchPosts(posts, query)[0].excerpt.flatMap((part) =>
    part.marked ? [part.text] : [],
  );

describe("searchPosts matching", () => {
  it("finds a word inside a longer Korean word", () => {
    const posts = [
      post(1, "A", "시간복잡도는 O(n)이다"),
      post(2, "B", "공간 복잡도를 줄인다"),
      post(3, "C", "상관없는 글"),
    ];

    expect(numbers(posts, "복잡도")).toEqual([2, 1]);
  });

  it("needs every word, wherever each one is and however the post spaces them", () => {
    const posts = [
      post(1, "A", "해시맵으로 센다"),
      post(2, "B", "해시 테이블"),
      post(3, "Hash map", "맵"),
    ];

    expect(numbers(posts, "해시 맵")).toEqual([1]);
    expect(numbers(posts, "hash 맵")).toEqual([3]);
  });

  it("ignores case, and treats the query as text rather than a pattern", () => {
    const posts = [
      post(1, "Two Sum", "dp[i] = dp[i - 1]"),
      post(2, "B", "dpi"),
    ];

    expect(numbers(posts, "TWO sum")).toEqual([1]);
    expect(numbers(posts, "dp[i]")).toEqual([1]);
  });

  it("matches text written in a different Unicode normalization", () => {
    const posts = [post(1, "A", "한글")];

    expect(numbers(posts, "한글".normalize("NFD"))).toEqual([1]);
  });

  it("returns nothing for a blank query", () => {
    expect(searchPosts([post(1, "A", "text")], "  ")).toEqual([]);
  });
});

describe("searchPosts while Korean is being typed", () => {
  const posts = [
    post(1, "A", "배열을 정렬한다"),
    post(2, "B", "트리를 순회한다"),
    post(3, "C", "트릭을 쓴다"),
    post(4, "D", "시간이 걸린다"),
    post(5, "E", "식을 세운다"),
  ];
  const composing = (query: string): number[] =>
    searchPosts(posts, query, { composing: true }).map(
      (result) => result.number,
    );

  it("reads the syllable still being composed as what it can become", () => {
    expect(composing("뱅")).toEqual([1]);
    expect(composing("배여")).toEqual([1]);
  });

  it("lists the posts that match as typed first, then the ones the word may still become", () => {
    // typing 시간 passes through 식
    expect(composing("식")).toEqual([5, 4]);
    expect(composing("트리")).toEqual([2, 3]);
  });

  it("matches only as typed once composing ends, as for a query opened from a link", () => {
    expect(numbers(posts, "뱅")).toEqual([]);
    expect(numbers(posts, "트리")).toEqual([2]);
  });
});

describe("searchPosts ranking", () => {
  it("puts a title match first", () => {
    const posts = [
      post(1, "Other", "sort sort sort"),
      post(2, "Sort an Array", "text"),
    ];

    expect(numbers(posts, "sort")).toEqual([2, 1]);
  });

  it("ranks a word that starts with the query above one that only contains it", () => {
    const posts = [
      post(1, "A", "조합을 구하고 집합을 만든다"),
      post(2, "B", "합을 구한다"),
    ];

    expect(numbers(posts, "합")).toEqual([2, 1]);
  });

  it("keeps the given order, newest first, between equal matches", () => {
    const posts = [
      post(3, "A", "정렬"),
      post(2, "B", "정렬"),
      post(1, "C", "정렬"),
    ];

    expect(numbers(posts, "정렬")).toEqual([3, 2, 1]);
  });
});

describe("searchPosts excerpt", () => {
  it("marks every query word in the excerpt, as it is written in the post", () => {
    const posts = [
      post(1, "A", "Binary Search 는 정렬된 배열에서 쓰는 Search 다"),
    ];

    expect(marked(posts, "search 배열")).toEqual(["Search", "배열", "Search"]);
  });

  it("shows the part of a long post where the query words are close together", () => {
    const filler = "문장 ".repeat(100);
    const posts = [
      post(1, "A", `시간 ${filler}복잡도 ${filler}시간복잡도는 O(n) ${filler}`),
    ];
    const excerpt = searchPosts(posts, "시간 복잡도")[0].excerpt;
    const text = excerpt.map((part) => part.text).join("");

    expect(text).toContain("시간복잡도는 O(n)");
    expect(text.startsWith("…")).toBe(true);
    expect(text.endsWith("…")).toBe(true);
    expect(text.length).toBeLessThan(200);
  });

  it("starts at the beginning of the post when only the title matches", () => {
    const posts = [post(1, "Two Sum", "배열에서 두 수를 찾는다")];
    const [result] = searchPosts(posts, "two");

    expect(result.excerpt).toEqual([
      { text: "배열에서 두 수를 찾는다", marked: false },
    ]);
  });

  it("fills the excerpt from before a match near the end of a long post", () => {
    const posts = [post(1, "A", `${"문장 ".repeat(200)}끝단어`)];
    const text = searchPosts(posts, "끝단어")[0]
      .excerpt.map((part) => part.text)
      .join("");

    expect(text.length).toBeGreaterThan(150);
    expect(text.endsWith("끝단어")).toBe(true);
  });

  it("doesn't cut a character written as two code units in half", () => {
    const posts = [
      post(1, "A", `${"😀".repeat(100)}x배열y${"😀".repeat(100)}`),
    ];
    const text = searchPosts(posts, "배열")[0]
      .excerpt.map((part) => part.text)
      .join("");

    expect(text).toContain("배열");
    expect(text).not.toMatch(/(?:^|[^\ud800-\udbff])[\udc00-\udfff]/);
    expect(text).not.toMatch(/[\ud800-\udbff](?![\udc00-\udfff])/);
  });

  it("marks overlapping words as one run", () => {
    const posts = [post(1, "A", "시간복잡도")];

    expect(marked(posts, "시간복 복잡도")).toEqual(["시간복잡도"]);
  });
});
