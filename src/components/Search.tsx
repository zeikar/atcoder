import {
  useDeferredValue,
  useEffect,
  useMemo,
  useState,
  type ChangeEvent,
} from "react";
import { postPath, withBase } from "../lib/links";
import { searchPosts, type SearchPost } from "../lib/search";

interface Props {
  base: string;
}

export default function Search({ base }: Props) {
  const [query, setQuery] = useState("");
  const [posts, setPosts] = useState<SearchPost[] | null>(null);
  const [failed, setFailed] = useState(false);
  // whether the last change came while a Korean IME held the last syllable open (see searchPosts). Taken from that input
  // event rather than compositionend: pressing the mouse on a result ends the composition before the click lands, and
  // the list mustn't shrink under the pointer; the next change that isn't composing resets it
  const [composing, setComposing] = useState(false);
  const term = query.trim();
  // matching takes about a millisecond, but a query as short as one letter lists every post, and rendering that list can
  // take a slow phone a tenth of a second; deferred, it never holds up the next keystroke
  const listedQuery = useDeferredValue(query);
  const listedComposing = useDeferredValue(composing);
  const listedTerm = listedQuery.trim();
  const results = useMemo(
    () =>
      posts === null
        ? []
        : searchPosts(posts, listedQuery, { composing: listedComposing }),
    [posts, listedQuery, listedComposing],
  );
  // search.json loads once, when the page opens; a query typed or opened with ?q= before it arrives waits for it
  const searching = term !== "" && posts === null && !failed;

  useEffect(() => {
    setQuery(new URLSearchParams(location.search).get("q") ?? "");
    fetch(withBase(base, "/search.json"))
      .then((response) => {
        if (!response.ok) {
          throw new Error(`search.json: ${response.status}`);
        }
        return response.json();
      })
      .then(setPosts, (error: unknown) => {
        console.error(error);
        setFailed(true);
      });
  }, [base]);

  const onChange = (event: ChangeEvent<HTMLInputElement>) => {
    const next = event.target.value;
    setQuery(next);
    setComposing((event.nativeEvent as InputEvent).isComposing === true);
    const url = new URL(location.href);
    if (next === "") {
      url.searchParams.delete("q");
    } else {
      url.searchParams.set("q", next);
    }
    history.replaceState(history.state, "", url);
  };

  return (
    <div>
      <label htmlFor="search-query" className="sr-only">
        Search posts
      </label>
      <div className="relative">
        <input
          id="search-query"
          type="search"
          value={query}
          onChange={onChange}
          placeholder="Search posts"
          className="w-full rounded-md border border-transparent bg-surface px-3 py-2 placeholder:text-muted focus:border-fg focus:outline-none"
        />
        {searching && (
          // below the input rather than on its edge, where the focus border, drawn in the same color, would hide it
          <div className="absolute inset-x-0 top-full mt-1 h-0.5 overflow-hidden">
            {/* the status line below says the same in words */}
            <div className="h-full w-2/5 animate-sweep bg-fg motion-reduce:w-full motion-reduce:animate-none" />
          </div>
        )}
      </div>
      <p role="status" className="mt-6 text-sm text-muted">
        {failed
          ? "Search failed. Reload the page to try again."
          : searching
            ? "Searching…"
            : posts !== null &&
              listedTerm !== "" &&
              `${results.length} ${results.length === 1 ? "result" : "results"} for "${listedTerm}"`}
      </p>
      {results.length > 0 && (
        <ul className="mt-4 border-t border-rule">
          {results.map((result) => (
            <li
              key={result.number}
              className="border-b border-rule py-4 break-keep break-words"
            >
              <a
                href={withBase(base, postPath(result.number))}
                // the press keeps focus in the search box: taking it away would end a composition the IME still holds,
                // and Chrome then scrolls the box back into view, so the link has moved before the click completes
                onMouseDown={(event) => event.preventDefault()}
                className="marker-draw text-[1.0625rem] font-semibold hover:stroke-drawn"
              >
                {result.title}
              </a>
              <p className="mt-1 text-sm text-muted">
                {result.excerpt.map((part, index) =>
                  part.marked ? (
                    <mark key={index}>{part.text}</mark>
                  ) : (
                    part.text
                  ),
                )}
              </p>
            </li>
          ))}
        </ul>
      )}
    </div>
  );
}
