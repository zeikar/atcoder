// this module only runs in Node at build time, and the project has no @types/node
declare const process: { env: Record<string, string | undefined> };

const GITHUB_GRAPHQL_URL = "https://api.github.com/graphql";

// a GitHub 5xx or a dropped connection is usually momentary, and a failed build on an issue or discussion event leaves
// the site stale until the next event, so such a request is tried once more; a 4xx (a bad token, a rate limit) would fail again
const RETRY_DELAY_MS = 5000;

type Reply = { status: number; text: string };

// one attempt reads the whole body: fetch resolves once the headers arrive, and the connection can still drop after that
const post = async (token: string, body: string): Promise<Reply> => {
  const response = await fetch(GITHUB_GRAPHQL_URL, {
    method: "POST",
    headers: {
      Authorization: `Bearer ${token}`,
      "Content-Type": "application/json",
      // without it repository and category ids come back in the deprecated legacy format
      "X-Github-Next-Global-ID": "1",
    },
    body,
  });
  return { status: response.status, text: await response.text() };
};

const postWithRetry = async (token: string, body: string): Promise<Reply> => {
  let failure: string;
  try {
    const reply = await post(token, body);
    if (reply.status < 500) {
      return reply;
    }
    failure = `status ${reply.status}`;
  } catch (error) {
    failure = String(error);
  }
  console.warn(
    `GitHub GraphQL request failed with ${failure}; retrying in ${RETRY_DELAY_MS / 1000}s`,
  );
  await new Promise((resolve) => setTimeout(resolve, RETRY_DELAY_MS));
  return post(token, body);
};

export const githubGraphql = async <T>(
  query: string,
  variables: Record<string, unknown>,
): Promise<T> => {
  const token = process.env.GITHUB_TOKEN;
  if (!token) {
    throw new Error(
      "GITHUB_TOKEN is not set: posts are fetched from the GitHub GraphQL API at build time, e.g. GITHUB_TOKEN=$(gh auth token) npm run build",
    );
  }

  const { status, text } = await postWithRetry(
    token,
    JSON.stringify({ query, variables }),
  );
  if (status !== 200) {
    throw new Error(`GitHub GraphQL request failed with ${status}: ${text}`);
  }

  const { data, errors } = JSON.parse(text) as {
    data: T;
    errors?: { type?: string; message: string }[];
  };
  // GraphQL reports a missing repository or user, and an exhausted primary rate limit (type RATE_LIMITED), with status 200;
  // secondary rate limits arrive as 403 or 429 instead
  if (errors && errors.length > 0) {
    const messages = errors.map(({ type, message }) =>
      type ? `${type}: ${message}` : message,
    );
    throw new Error(`GitHub GraphQL errors: ${messages.join("; ")}`);
  }
  return data;
};

export type Label = { name: string; color: string };

// Issue and Discussion share these fields, but their open state differs: Issue has state, Discussion has closed and category
const POST_FIELDS = `
  __typename number title body createdAt lastEditedAt url
  author { login }
  repository { nameWithOwner }
  labels(first: 100) { nodes { name color } }
  comments { totalCount }
`;

// untrusted until isTrustedPost passes: repository listings include posts by anyone
export type PostNode = {
  __typename: string;
  number: number;
  title: string;
  body: string | null;
  createdAt: string;
  // null until the body is edited
  lastEditedAt: string | null;
  url: string;
  // null when the author's account was deleted
  author: { login: string } | null;
  repository: { nameWithOwner: string };
  labels: { nodes: Label[] };
  comments: { totalCount: number };
  state?: string;
  closed?: boolean;
  category?: DiscussionCategory;
};

type PostPage = {
  repository: {
    nameWithOwner: string;
    posts: {
      pageInfo: { hasNextPage: boolean; endCursor: string | null };
      nodes: PostNode[];
    };
  };
};

// both queries alias their connection as "posts" so one pager serves issues and discussions
const fetchAllPosts = async (
  owner: string,
  repo: string,
  query: string,
  variables: Record<string, unknown> = {},
): Promise<PostNode[]> => {
  const nodes: PostNode[] = [];
  let cursor: string | null = null;
  do {
    const page: PostPage = await githubGraphql<PostPage>(query, {
      owner,
      repo,
      ...variables,
      cursor,
    });
    const { nameWithOwner } = page.repository;
    // GitHub follows a renamed or transferred repository, whose posts then all fail isTrustedPost and the build would go green with no posts
    if (nameWithOwner.toLowerCase() !== `${owner}/${repo}`.toLowerCase()) {
      throw new Error(
        `config.json: ${owner}/${repo} now resolves to ${nameWithOwner}; update "repoOwner"/"repoName"`,
      );
    }
    const { pageInfo } = page.repository.posts;
    nodes.push(...page.repository.posts.nodes);
    cursor = pageInfo.hasNextPage ? pageInfo.endCursor : null;
  } while (cursor !== null);
  return nodes;
};

export const fetchIssues = (owner: string, repo: string): Promise<PostNode[]> =>
  fetchAllPosts(
    owner,
    repo,
    `query ($owner: String!, $repo: String!, $cursor: String) {
      repository(owner: $owner, name: $repo) {
        nameWithOwner
        posts: issues(states: OPEN, filterBy: { createdBy: $owner }, first: 100, after: $cursor) {
          pageInfo { hasNextPage endCursor }
          nodes { ${POST_FIELDS} state }
        }
      }
    }`,
  );

export type DiscussionCategory = { id: string; name: string; slug: string };

// checks the configured category slugs, so a typo fails the build instead of publishing nothing, and returns their ids
// along with the repository id giscus needs
export const fetchDiscussionCategories = async (
  owner: string,
  repo: string,
  slugs: string[],
): Promise<{ repoId: string; categoryIds: string[] }> => {
  const { repository } = await githubGraphql<{
    repository: {
      id: string;
      hasDiscussionsEnabled: boolean;
      discussionCategories: { nodes: { id: string; slug: string }[] };
    };
  }>(
    `query ($owner: String!, $repo: String!) {
      repository(owner: $owner, name: $repo) {
        id
        hasDiscussionsEnabled
        discussionCategories(first: 100) { nodes { id slug } }
      }
    }`,
    { owner, repo },
  );

  // a repository made from the template starts with Discussions off, and would otherwise only hear that no slug matches
  if (!repository.hasDiscussionsEnabled) {
    throw new Error(
      `config.json: "source" is "discussions", but Discussions are turned off in ${owner}/${repo}; turn them on in its Settings under Features, or set "source": "issues"`,
    );
  }
  const categories = repository.discussionCategories.nodes;
  const available = categories.map((category) => category.slug);
  const missing = slugs.filter((slug) => !available.includes(slug));
  if (missing.length > 0) {
    throw new Error(
      `config.json: "discussionCategory" names slugs that aren't discussion categories in ${owner}/${repo}: ${missing.map((slug) => `"${slug}"`).join(", ")}; available slugs: ${available.join(", ") || "none"}`,
    );
  }
  return {
    repoId: repository.id,
    // each category once, however often its slug is listed
    categoryIds: categories
      .filter((category) => slugs.includes(category.slug))
      .map((category) => category.id),
  };
};

// a category's discussions, oldest first: the default order is by last update, and a discussion updated between two
// page requests would jump over the cursor and be skipped
export const fetchDiscussions = (
  owner: string,
  repo: string,
  categoryId: string,
): Promise<PostNode[]> =>
  fetchAllPosts(
    owner,
    repo,
    `query ($owner: String!, $repo: String!, $categoryId: ID!, $cursor: String) {
      repository(owner: $owner, name: $repo) {
        nameWithOwner
        posts: discussions(categoryId: $categoryId, states: OPEN, orderBy: { field: CREATED_AT, direction: ASC }, first: 100, after: $cursor) {
          pageInfo { hasNextPage endCursor }
          nodes { ${POST_FIELDS} closed category { id name slug } }
        }
      }
    }`,
    { categoryId },
  );

// the posts pinned to the top of the repository's issue or discussion list. GitHub allows only a few pins, so one page
// holds them all. Discussions pinned to a category alone aren't in the API, and a pin is only a number here: the
// posts themselves still have to pass isTrustedPost
export const fetchPinnedNumbers = async (
  owner: string,
  repo: string,
  source: "issues" | "discussions",
): Promise<Set<number>> => {
  const pins =
    source === "issues"
      ? "pinnedIssues(first: 100) { nodes { post: issue { number } } }"
      : "pinnedDiscussions(first: 100) { nodes { post: discussion { number } } }";
  const { repository } = await githubGraphql<{
    repository: { pins: { nodes: { post: { number: number } }[] } };
  }>(
    `query ($owner: String!, $repo: String!) {
      repository(owner: $owner, name: $repo) { pins: ${pins} }
    }`,
    { owner, repo },
  );
  return new Set(repository.pins.nodes.map(({ post }) => post.number));
};

// the repository's About text, which describes the site when there is one
export const fetchRepositoryDescription = async (
  owner: string,
  repo: string,
): Promise<string | null> => {
  const { repository } = await githubGraphql<{
    repository: { description: string | null };
  }>(
    `query ($owner: String!, $repo: String!) {
      repository(owner: $owner, name: $repo) { description }
    }`,
    { owner, repo },
  );
  return repository.description?.trim() || null;
};

type Profile = {
  login: string;
  name: string | null;
  bio: string | null;
  avatarUrl: string;
  url: string;
  followers: { totalCount: number };
  following: { totalCount: number };
};

export const fetchProfile = async (login: string): Promise<Profile> => {
  // repositoryOwner rather than user, which answers an organization with a bare NOT_FOUND error
  const { repositoryOwner } = await githubGraphql<{
    repositoryOwner:
      ({ __typename: "User" } & Profile) | { __typename: string } | null;
  }>(
    `query ($login: String!) {
      repositoryOwner(login: $login) {
        __typename
        ... on User {
          login name bio avatarUrl url
          followers { totalCount }
          following { totalCount }
        }
      }
    }`,
    { login },
  );
  if (repositoryOwner === null) {
    throw new Error(
      `config.json: there is no GitHub account named "${login}" for "repoOwner"`,
    );
  }
  if (!("login" in repositoryOwner) || repositoryOwner.__typename !== "User") {
    // only posts written by repoOwner are published, and an organization writes none. repoOwner also has to name the
    // repository the build runs in, so the fix is moving the repository, not changing the setting
    throw new Error(
      `config.json: "repoOwner" "${login}" is an organization, which writes no issues or discussions of its own; Repozine publishes its owner's posts, so the repository has to belong to a user account`,
    );
  }
  const { __typename: _, ...profile } = repositoryOwner;
  return profile;
};
