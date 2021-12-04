#include <iostream>
#include <vector>
#include <queue>
using namespace std;

vector<int> edges[200001];
int indegree[200001];

int main() {
	int n, m;

	cin >> n >> m;

	while (m--)
	{
		int a, b;
		cin >> a >> b;

		edges[a].push_back(b);
		++indegree[b];
	}

	priority_queue<int> que;
	queue<int> ans;
	for (int i = 1; i <= n; i++)
	{
		if (indegree[i] == 0)
		{
			que.push(-i);
		}
	}

	while (!que.empty())
	{
		int node = -que.top();
		que.pop();
		ans.push(node);

		for (int i = 0; i < edges[node].size(); i++)
		{
			int next = edges[node][i];

			indegree[next]--;

			if (indegree[next] == 0)
			{
				que.push(-next);
			}
		}
	}

	if (ans.size() != n)
	{
		cout << -1 << endl;
	}
	else
	{
		while (!ans.empty())
		{
			cout << ans.front() << ' ';
			ans.pop();
		}
		cout << endl;
	}
}