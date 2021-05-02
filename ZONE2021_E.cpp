#include <iostream>
#include <queue>
using namespace std;

int a[501][501];
int b[501][501];
bool visited[501][501];
int dist[501][501];

int main() {
	int R, C;
	cin >> R >> C;

	for (int i = 1; i <= R; i++)
	{
		for (int j = 1; j <= C - 1; j++)
		{
			cin >> a[i][j];
		}
	}
	for (int i = 1; i <= R - 1; i++)
	{
		for (int j = 1; j <= C; j++)
		{
			cin >> b[i][j];
		}
	}
	for (int i = 1; i <= R; i++)
	{
		for (int j = 1; j <= C; j++)
		{
			dist[i][j] = 987654321;
		}
	}

	int res = -1;
	priority_queue<pair<int, pair<int, int> > > pq;
	pq.push(make_pair(0, make_pair(1, 1)));
	dist[1][1] = 0;

	while (!pq.empty())
	{
		int cost = -pq.top().first;
		int r = pq.top().second.first;
		int c = pq.top().second.second;
		pq.pop();

		if (visited[r][c])
		{
			continue;
		}

		if (r == R && c == C)
		{
			res = cost;
			break;
		}

		visited[r][c] = true;

		if (c < C && !visited[r][c + 1] && dist[r][c] + a[r][c] < dist[r][c + 1])
		{
			dist[r][c + 1] = dist[r][c] + a[r][c];
			pq.push(make_pair(-(cost + a[r][c]), make_pair(r, c + 1)));
		}
		if (c > 1 && !visited[r][c - 1] && dist[r][c] + a[r][c - 1] < dist[r][c - 1])
		{
			dist[r][c - 1] = dist[r][c] + a[r][c - 1];
			pq.push(make_pair(-(cost + a[r][c - 1]), make_pair(r, c - 1)));
		}
		if (r < R && !visited[r + 1][c] && dist[r][c] + b[r][c] < dist[r + 1][c])
		{
			dist[r + 1][c] = dist[r][c] + b[r][c];
			pq.push(make_pair(-(cost + b[r][c]), make_pair(r + 1, c)));
		}

		for (int i = 1; i < r; i++)
		{
			if (visited[r - i][c] == false && dist[r][c] + 1 + i < dist[r - i][c])
			{
				dist[r - i][c] = dist[r][c] + 1 + i;
				pq.push(make_pair(-(cost + 1 + i), make_pair(r - i, c)));
			}
		}
	}

	cout << res << endl;
}