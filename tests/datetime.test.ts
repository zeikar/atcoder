import { describe, expect, it } from "vitest";
import { editedADayLater, formatDate } from "../src/lib/datetime";

describe("editedADayLater", () => {
  it.each([
    ["never edited", "2026-11-01T04:30:00Z", false],
    ["edited within the day", "2026-11-02T04:29:59Z", false],
    // posted at 00:30 in New York on the day clocks fall back, so 24 hours later is 23:30 on the same date there
    ["edited 24 hours later", "2026-11-02T04:30:00Z", false],
    ["edited 25 hours later", "2026-11-02T05:30:00Z", true],
    ["edited months later", "2027-03-01T00:00:00Z", true],
  ])("%s", (_, updatedAt, expected) => {
    expect(editedADayLater("2026-11-01T04:30:00Z", updatedAt)).toBe(expected);
  });
});

describe("formatDate", () => {
  // both ends of the UTC day, so a build machine on either side of UTC would shift one of them
  it.each(["2021-02-18T00:00:00Z", "2021-02-18T23:59:59Z"])(
    "formats %s as its UTC date",
    (dateString) => {
      expect(formatDate(dateString, "long", "en")).toBe("February 18, 2021");
      expect(formatDate(dateString, "short", "en")).toBe("Feb 18, 2021");
    },
  );

  it("writes the date in the site's language", () => {
    expect(formatDate("2021-02-18T00:00:00Z", "long", "ko-KR")).toBe(
      "2021년 2월 18일",
    );
  });
});
