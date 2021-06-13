#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
using namespace std;

vector<int> edges[2001];
long long res = 0;
bool visited[2001];

void bfs(int start) {
	memset(visited, false, sizeof(visited));

	queue<int> que;
	que.push(start);
	visited[start] = true;

	while (!que.empty())
	{
		int node = que.front();
		que.pop();
		++res;

		for (int i = 0; i < edges[node].size(); i++)
		{
			if (visited[edges[node][i]])
			{
				continue;
			}

			que.push(edges[node][i]);
			visited[edges[node][i]] = true;
		}
	}
}

int main() {
	int n, m;

	cin >> n >> m;

	for (int i = 0; i < m; i++)
	{
		int a, b;
		cin >> a >> b;
		edges[a].push_back(b);
	}

	for (int i = 1; i <= n; i++)
	{
		bfs(i);
	}

	cout << res << endl;
}