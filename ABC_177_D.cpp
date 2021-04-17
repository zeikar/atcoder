#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<int> edges[200001];
bool visited[200001];

int dfs(int node) {
	if (visited[node])
	{
		return 0;
	}

	int ret = 1;
	visited[node] = true;

	for (int i = 0; i < edges[node].size(); i++)
	{
		int next = edges[node][i];

		if (visited[next])
		{
			continue;
		}

		ret += dfs(next);
	}

	return ret;
}

int main() {
	int n, m;

	cin >> n >> m;

	for (int i = 0; i < m; i++)
	{
		int a, b;
		cin >> a >> b;

		edges[a].push_back(b);
		edges[b].push_back(a);
	}

	int result = 1;
	for (int i = 1; i <= n; i++)
	{
		result = max(result, dfs(i));
	}

	cout << result << endl;
}