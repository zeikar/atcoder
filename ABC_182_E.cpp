#include <iostream>
#include <vector>
#include <cstring>
#include <utility>
using namespace std;

vector<pair<int, int> > bulbs;
int grid[1502][1502];
int h, w, n, m;

const int direction[][2] = { 1, 0, -1, 0, 0, 1, 0, -1 };

int main() {
	cin >> h >> w >> n >> m;

	// set border to -1
	for (int i = 0; i <= h; i++)
	{
		grid[i][0] = -1;
		grid[i][w + 1] = -1;
	}
	for (int i = 0; i <= w; i++)
	{
		grid[0][i] = -1;
		grid[h + 1][i] = -1;
	}

	for (int i = 0; i < n; i++)
	{
		int a, b;
		cin >> a >> b;

		grid[a][b] = 1;
		bulbs.push_back(make_pair(a, b));
	}

	for (int i = 0; i < m; i++)
	{
		int c, d;
		cin >> c >> d;

		grid[c][d] = -1;
	}

	int ans = n;

	for (int i = 0; i < n; i++)
	{
		int a = bulbs[i].first;
		int b = bulbs[i].second;

		for (int d = 0; d < 4; d++)
		{
			int x = a, y = b;
			int cnt = 1;

			while (true)
			{
				int nx = x + direction[d][0] * cnt;
				int ny = y + direction[d][1] * cnt;

				if (grid[nx][ny] == -1 || grid[nx][ny] == 1)
				{
					break;
				}

				if (grid[nx][ny] == 0)
				{
					ans++;
				}
				grid[nx][ny] = 2;
				cnt++;
			}
		}
	}

	cout << ans << endl;
}