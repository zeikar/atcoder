#include <iostream>
#include <queue>
#include <vector>
using namespace std;

vector<int> edges[100001];
int sign[100001];

int main() {
	int n, m;

	cin >> n >> m;

	while (m--)
	{
		int a, b;

		cin >> a >> b;

		edges[a].push_back(b);
		edges[b].push_back(a);
	}

	queue<int> que;

	que.push(1);

	while (!que.empty())
	{
		int c = que.front();
		que.pop();

		for (int i = 0; i < edges[c].size(); i++)
		{
			int next = edges[c][i];

			if (sign[next] != 0)
			{
				continue;
			}

			sign[next] = c;

			que.push(next);
		}
	}

	// check
	for (int i = 1; i <= n; i++)
	{
		if (sign[i] == 0)
		{
			cout << "No" << endl;
			return 0;
		}
	}

	cout << "Yes" << endl;
	for (int i = 2; i <= n; i++)
	{
		cout << sign[i] << endl;
	}
}