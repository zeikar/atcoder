#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <utility>
#include <queue>
using namespace std;

bool isVisited[100001];
vector<int> edges[100001];

void bfs(int node)
{
	queue<int> que;

	que.push(node);
	isVisited[node] = true;

	while (!que.empty())
	{
		node = que.front();
		que.pop();

		for (int i = 0; i < edges[node].size(); i++)
		{
			int next = edges[node][i];
			if (isVisited[next])
			{
				continue;
			}

			isVisited[next] = true;
			que.push(next);
		}
	}
}

int main()
{
	int n, m;
	int chunks = 0;
	cin >> n >> m;

	while (m--)
	{
		int a, b;
		cin >> a >> b;

		edges[a].push_back(b);
		edges[b].push_back(a);
	}

	for (int i = 1; i <= n; i++)
	{
		if (isVisited[i] == false)
		{
			++chunks;

			bfs(i);
		}
	}

	cout << chunks - 1 << endl;
}