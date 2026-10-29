# Repozine

[![Deploy][deploy-shield]][deploy-url]
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]

<picture>
  <source media="(prefers-color-scheme: dark)" srcset=".github/assets/repozine-hero-dark.png" />
  <img alt="Repozine: turn your repo's issues into a blog" src=".github/assets/repozine-hero-light.png" />
</picture>

---

Repozine turns a GitHub repository's Issues or Discussions into a static blog or study log, built with Astro and deployed automatically to GitHub Pages.

[Gmeek](https://github.com/Meekdai/Gmeek) and [gitblog](https://github.com/yihong0618/gitblog) publish a blog from Issues, and [Hakuba](https://github.com/YeungKC/Hakuba) from Discussions. Repozine publishes from either, and it can live on its own branch of a repository that already holds your code, so the posts sit next to the work they describe.

# Sites using Repozine

- [zeikar.dev/leetcode](https://zeikar.dev/leetcode/): 160+ LeetCode solution notes written since 2021, published from the Discussions of [zeikar/leetcode](https://github.com/zeikar/leetcode), the repository that holds the solutions
- [zeikar.dev/atcoder](https://zeikar.dev/atcoder/): 30+ AtCoder solution notes from [zeikar/atcoder](https://github.com/zeikar/atcoder), set up the same way
- [zeikar.dev/repozine](https://zeikar.dev/repozine/): the demo, with one post

# How to use

Setting up a site takes three steps: get Repozine into a repository, prepare the issues or discussions it publishes, then fill in `config.json` and push.

## 1. Get the code

Repozine can be a repository of its own, made from this template, or a `repozine` branch of a repository that already holds your code.

### A new repository from the template

[Use this template](https://github.com/zeikar/repozine/generate) to create a repository. [docs](https://docs.github.com/en/repositories/creating-and-managing-repositories/creating-a-repository-from-a-template#creating-a-repository-from-a-template)

In its Settings → **Pages**, set the source to **GitHub Actions**. [docs](https://docs.github.com/en/pages/getting-started-with-github-pages/configuring-a-publishing-source-for-your-github-pages-site)

### A branch of an existing repository

In the repository's Settings:

1. **Pages:** set the source to **GitHub Actions**. [docs](https://docs.github.com/en/pages/getting-started-with-github-pages/configuring-a-publishing-source-for-your-github-pages-site)
2. **Environments → github-pages:** add `repozine` to the deployment branches, since the switch above allows only the default branch.
3. **Secrets and variables → Actions → Variables:** set `REPOZINE_REF` to `repozine`.

Then create the branch from Repozine. Commit or stash your changes first: `-f` discards uncommitted work without asking.

```bash
git remote add repozine https://github.com/zeikar/repozine
git fetch repozine
git checkout -f -b repozine repozine/main
```

## 2. Choose where posts come from

Posts are the open issues or discussions written by the repository's owner, and their labels become tags. Set `source` in `config.json` to one of the two.

- **Issues:** every open issue you write is a post. For comments, install the [utterances GitHub App](https://github.com/apps/utterances) on the repository.
- **Discussions:** every open discussion you write is a post, in any category. In Settings → **General → Features**, turn on **Discussions**, and for comments, install the [giscus GitHub App](https://github.com/apps/giscus) on the repository.
  - If you posted GitHub's "Welcome to … Discussions!" while setting Discussions up, close or delete it, or it becomes a post too.
  - To publish only some categories, keeping posts apart from your Q&A questions or polls, list their slugs in `discussionCategory`. A slug is the end of the category's URL, `posts` in `.../discussions/categories/posts`. A category in the Announcement format lets only maintainers start discussions in it, and naming categories limits what each build downloads to the discussions in them.

Comments are bound to the post's number, so each post keeps the comments made on its issue or discussion.

## 3. Configure and deploy

Edit `config.json`:

```json
{
  "websiteTitle": "My Blog",
  "repoOwner": "your-username",
  "repoName": "your-repo",
  "source": "discussions",
  "googleAnalyticsId": "",
  "language": "en"
}
```

| Key                  | Description                                                                                                                                                                                                                                                                                                |
| -------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `websiteTitle`       | Title shown in the navbar, footer and browser tab                                                                                                                                                                                                                                                          |
| `repoOwner`          | Owner of the repository. Only issues or discussions authored by this account are published, so the repository must be owned by a user, not an organization                                                                                                                                                 |
| `repoName`           | Repository whose issues or discussions become posts. A build in GitHub Actions stops if `repoOwner`/`repoName` name a different repository from the one it runs in                                                                                                                                         |
| `source`             | `"issues"` or `"discussions"`                                                                                                                                                                                                                                                                              |
| `discussionCategory` | Discussion category **slug** to publish from, or a list of them such as `["posts", "notes"]`. Leave it out to publish every category                                                                                                                                                                       |
| `googleAnalyticsId`  | Google Analytics measurement ID (e.g. `G-XXXXXXXXXX`). Leave empty to disable analytics                                                                                                                                                                                                                    |
| `language`           | Language of the posts as a tag such as `"en"` or `"ko"`, used for `<html lang>`, dates and the giscus interface (English when giscus has no translation). Include a region (`"ko-KR"`) to also set `og:locale`. Korean (`"ko"`, `"ko-KR"`) also loads the Hangul faces of the web font. Defaults to `"en"` |

Then commit it and push to the branch Repozine is on, which runs the deploy workflow: it builds the site and publishes it to GitHub Pages.

```bash
git commit -am "Configure Repozine"
git push origin main   # git push origin repozine on a branch of an existing repository
```

### Rebuild when posts change

A repository made from the template already rebuilds the site on issue, discussion, comment and label events (see [Known limitations](#known-limitations)). On a branch of an existing repository, issue and discussion events only run workflows from the default branch, so add this workflow to your default branch as `.github/workflows/deploy.yml`, with `OWNER/REPO` replaced by your repository. It runs the deploy workflow from your `repozine` branch, so updating Repozine updates it too. That workflow still checks out the code to build from `REPOZINE_REF` (step 1); without it, it would build the default branch, which has no site in it.

<!-- prettier-ignore -->
```yaml
name: Deploy to GitHub Pages

on:
  issues:
    types: [opened, edited, deleted, closed, reopened, labeled, unlabeled, transferred, pinned, unpinned]
  issue_comment:
    types: [created, deleted]
  discussion:
    types: [created, edited, deleted, transferred, category_changed, labeled, unlabeled, pinned, unpinned]
  discussion_comment:
    types: [created, deleted]
  label:
    types: [edited, deleted]
  workflow_dispatch:

jobs:
  deploy:
    uses: OWNER/REPO/.github/workflows/deploy.yml@repozine
    permissions:
      contents: read
      issues: read
      discussions: read
      pages: write
      id-token: write
```

## Updating Repozine

Merge the latest Repozine into the branch your site builds from, keeping your own `config.json`:

```bash
git remote add repozine https://github.com/zeikar/repozine   # skip if already added
git switch repozine   # main in a repository made from the template
git fetch repozine
git merge --no-commit repozine/main
git checkout HEAD -- config.json
git commit -m "Update Repozine"
git push origin HEAD
```

The merge doesn't touch the rebuild workflow on your default branch, if you added one. Compare it with the one [above](#rebuild-when-posts-change), which may have gained events, such as `label` or `pinned`.

A repository made from the template shares no history with Repozine until its first update. For that one, in place of the merge line, run these two, which replace your files with Repozine's. Commit or stash your changes first: unlike a merge, they overwrite uncommitted work without asking.

```bash
git merge --no-commit --allow-unrelated-histories -s ours repozine/main
git read-tree -u --reset repozine/main
```

Before committing, `git status` shows what they delete: files Repozine no longer has, and any you added yourself, which `git checkout HEAD -- <file>` keeps.

If a build then stops on a `config.json` key, add it; see [Configure and deploy](#3-configure-and-deploy).

## Switch from issues to discussions

GitHub has no API or bulk action for this, so convert each post with **Convert to discussion** on its issue page, into a category you publish. The discussion keeps the title, body, author, labels and date but gets a new number, so the post's URL changes; the issue is left closed and locked. Then set `source` to `"discussions"` in `config.json`, and `discussionCategory` if you publish only some categories.

## Pinned posts

Pin one of your posts on GitHub (an issue or a discussion, whichever `source` publishes) to list it first on the home page; tag pages stay in date order. GitHub allows 3 pinned issues and 4 pinned discussions per repository. Only discussions pinned to the top of the Discussions page count, since pins to a single category aren't in GitHub's API.

## Search engines

- Every page gets Open Graph / Twitter card tags and, when the repository's About text or the owner's bio provides one, a description. Every page except search and 404 (both `noindex`) gets a canonical URL. Posts add their dates and labels as article metadata and schema.org `BlogPosting` data.
- A post's link preview shows its first image. Posts without one get a card rendered by [DOGimg](https://dogimg.vercel.app).
- `sitemap.xml` and `rss.xml` are built next to the home page, e.g. `https://<owner>.github.io/<repo>/sitemap.xml`. The feed carries only the newest 20 posts, in full, with their code unhighlighted.
- Crawlers read `robots.txt` only at the root of a host, so a site gets one only when it is served from there: a `<owner>.github.io` repository, or a project repository with its own domain. For a project site under a path, add `Sitemap: https://<host>/<repo>/sitemap.xml` to the root site's `robots.txt`, or submit the sitemap in Google Search Console.
- The deploy workflow passes the site's real address to the build, so absolute URLs and links follow a custom domain, including one set on the repository itself. Any other build, local or on another host such as Cloudflare Pages, assumes `https://<owner>.github.io/<repo>/` for its links and asset paths too, unless `SITE_URL` is set to the site's address, e.g. `SITE_URL=https://blog.example.com`.

## Local development

Posts are fetched from the GitHub GraphQL API at build time, so `npm run dev`, `npm run build` and `npm run check` all need a `GITHUB_TOKEN`.

```bash
npm install
GITHUB_TOKEN=$(gh auth token) npm run dev
```

# Known limitations

- A closed discussion is removed from the site only on the next build, because GitHub Actions has no discussion closed/reopened trigger.
- Without JavaScript, dates are shown in UTC; otherwise they are shown in the reader's time zone.
- Discussion comment counts include top-level comments only.
- The search page downloads the text of every post, which grows with the number of posts: about 70KB compressed for 160 posts.

# License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->

[deploy-shield]: https://github.com/zeikar/repozine/actions/workflows/deploy.yml/badge.svg
[deploy-url]: https://github.com/zeikar/repozine/actions/workflows/deploy.yml
[contributors-shield]: https://img.shields.io/github/contributors/zeikar/repozine.svg
[contributors-url]: https://github.com/zeikar/repozine/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/zeikar/repozine.svg
[forks-url]: https://github.com/zeikar/repozine/network/members
[stars-shield]: https://img.shields.io/github/stars/zeikar/repozine.svg
[stars-url]: https://github.com/zeikar/repozine/stargazers
[issues-shield]: https://img.shields.io/github/issues/zeikar/repozine.svg
[issues-url]: https://github.com/zeikar/repozine/issues
[license-shield]: https://img.shields.io/github/license/zeikar/repozine.svg
[license-url]: https://github.com/zeikar/repozine/blob/main/LICENSE.txt
