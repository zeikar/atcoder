#include <iostream>
#include <vector>
using namespace std;

int n, m;
vector<int> edges[20];
int colors[20];
int group[20];
vector<int> groups[21];
vector<int> newGroups[21];


const int RED = 1;
const int GREEN = 2;
const int BLUE = 3;

bool isSafe(int node, int color)
{
	for (int i = 0; i < edges[node].size(); i++)
	{
		int adj = edges[node][i];
		if (colors[adj] == color)
		{
			return 0;
		}
	}
	return 1;
}

long long dfs(int groupIdx, int nodeIdx) {
	if (nodeIdx >= groups[groupIdx].size() - 1)
	{
		return 1;
	}

	long long ret = 0;
	bool used[4] = { false };
	int node = groups[groupIdx][nodeIdx + 1];


	for (int c = 1; c <= 3; c++)
	{
		bool ok = true;
		for (int i = 0; i < edges[node].size(); i++)
		{
			int adj = edges[node][i];
			if (colors[adj] == c)
			{
				ok = false;
				break;
			}
		}

		if (ok)
		{
			colors[groups[groupIdx][nodeIdx + 1]] = c;
			ret += dfs(groupIdx, nodeIdx + 1);
			colors[groups[groupIdx][nodeIdx + 1]] = 0;
		}
	}

	return ret;
}

int grouping(int node, int g) {
	int ret = 1;

	for (int i = 0; i < edges[node].size(); i++)
	{
		int next = edges[node][i];

		if (group[next] == 0)
		{
			group[next] = g;
			groups[g].push_back(next);
			grouping(next, g);
		}
	}

	return ret;
}

int main() {

	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> n >> m;

	while (m--)
	{
		int a, b;
		cin >> a >> b;

		a--; b--;

		edges[a].push_back(b);
		edges[b].push_back(a);
	}

	for (int i = 0; i < n; i++)
	{
		if (group[i] == 0)
		{
			group[i] = i + 1;
			groups[i + 1].push_back(i);
			grouping(i, i + 1);
		}
	}

	long long res = 1;
	for (int i = 0; i <= n; i++)
	{
		long long temp = 0;

		if (groups[i].size() == 0)
		{
			continue;
		}

		for (int c = 1; c <= 1; c++)
		{
			colors[groups[i][0]] = 1;
			temp += 3 * dfs(i, 0);
			colors[groups[i][0]] = 0;
		}

		res *= temp;
	}
	cout << res << endl;
}