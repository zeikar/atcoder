// shared with the inline script in Base.astro, which re-renders dates in the reader's time zone
export const DATE_FORMATS = {
  long: { year: "numeric", month: "long", day: "numeric" },
  short: { year: "numeric", month: "short", day: "numeric" },
} as const;

export type DateFormat = keyof typeof DATE_FORMATS;

// a day, plus the hour that the day clocks fall back adds
const DATE_APART = 25 * 60 * 60 * 1000;

// a post shows its edit date only when the edit is far enough from posting to fall on a later date for readers in any
// time zone whose clocks fall back by at most an hour (all but Antarctica's Troll station); otherwise the same date
// could show twice
export const editedADayLater = (
  createdAt: string,
  updatedAt: string,
): boolean => Date.parse(updatedAt) - Date.parse(createdAt) >= DATE_APART;

// input: dateString (2021-02-18T14:50:29Z), and the site's language as the locale
export const formatDate = (
  dateString: string,
  format: DateFormat,
  locale: string,
): string => {
  return new Date(dateString).toLocaleDateString(locale, {
    ...DATE_FORMATS[format],
    // pages are rendered at build time, so the build machine's time zone must not shift the date;
    // this UTC date is what readers without JavaScript see
    timeZone: "UTC",
  });
};
