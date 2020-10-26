#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>
using namespace std;

int a[200001];
int b[200001];
vector<int> edges[200001];
bool visited[200001];

bool dfs(int start, int node, int diff) {
	//cout << start << ' ' << node << ' ' << diff << endl;
	if (start < node)
	{
		a[node] -= diff;
		a[start] += diff;
		return true;
	}
	if (start == node)
	{
		return false;
	}
	if (visited[node])
	{
		return false;
	}
	visited[node] = true;

	for (int i = 0; i < edges[node].size(); i++)
	{
		int next = edges[node][i];

		if (dfs(start, next, diff))
		{
			return true;
		}
	}
	return false;
}

int main() {
	int n, m;
	long long suma = 0, sumb = 0;

	cin >> n >> m;

	for (int i = 1; i <= n; i++)
	{
		cin >> a[i];
		suma += a[i];
	}
	for (int i = 1; i <= n; i++)
	{
		cin >> b[i];
		sumb += b[i];
	}

	while (m--)
	{
		int c, d;
		cin >> c >> d;
		edges[c].push_back(d);
		edges[d].push_back(c);
	}

	// case 1. sum of a, b must be same
	if (suma != sumb)
	{
		cout << "No" << endl;
		return 0;
	}

	// case 2.
	for (int i = 1; i <= n; i++)
	{
		if (a[i] == b[i])
		{
			continue;
		}

		int diff = b[i] - a[i];

		for (int j = 0; j < edges[i].size(); j++)
		{
			int next = edges[i][j];

			/*if (next < i)
			{
				continue;
			}

			a[next] -= diff;
			a[i] += diff;
			break;*/
			memset(visited, false, sizeof(visited));

			if (dfs(i, next, diff))
			{
				break;
			}
		}
	}

	// debug
	/*for (int i = 1; i <= n; i++)
	{
		cout << a[i] << ' ';
	}
	cout << endl;
	for (int i = 1; i <= n; i++)
	{
		cout << b[i] << ' ';
	}
	cout << endl;*/

	for (int i = 1; i <= n; i++)
	{
		if (a[i] != b[i])
		{
			cout << "No" << endl;
			return 0;
		}
	}

	cout << "Yes" << endl;

}