import { describe, expect, it } from "vitest";
import { composingPattern } from "../src/lib/hangul";

const matches = (word: string, text: string): boolean =>
  new RegExp(composingPattern(word), "u").test(text);

describe("composingPattern", () => {
  // what the search box holds, keystroke by keystroke, while an IME composes a word
  it.each([
    ["an initial consonant alone", "ㅂ", "배열"],
    ["a syllable that can still take a final", "배", "뱅"],
    ["a syllable whose vowel can still grow", "고", "과제"],
    ["a final that becomes the next syllable's initial", "뱅", "배열"],
    ["a syllable waiting for its vowel", "배여", "배열"],
    ["a consonant after finished syllables", "복잡ㄷ", "시간복잡도"],
    ["a final that can still grow into a compound", "달", "닭"],
    ["half of a compound final moving on", "닭", "달걀"],
  ])("matches %s", (_, word, text) => {
    expect(matches(word, text)).toBe(true);
  });

  it("keeps what is already typed", () => {
    expect(matches("배여", "배추")).toBe(false);
    expect(matches("뱅", "뱀")).toBe(false);
    expect(matches("복잡ㄷ", "복잡하다")).toBe(false);
  });

  it("matches a word without Hangul at its end literally, special characters included", () => {
    expect(composingPattern("c++")).toBe("c\\+\\+");
    expect(matches("dp[i]", "dp[i] = 1")).toBe(true);
  });
});
