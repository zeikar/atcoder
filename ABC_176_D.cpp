#include <iostream>
#include <string>
#include <deque>
#include <utility>
#include <cstring>
using namespace std;

int h, w;
int dx, dy;
string map[1000];
bool visited[1000][1000];
int level[1000][1000];

const int direction[][2] = { -1, 0, 0, -1, 0, 1, 1, 0 };

int main() {
	int cx, cy;

	cin >> h >> w >> cx >> cy >> dx >> dy;

	--cx;
	--cy;
	--dx;
	--dy;

	for (int i = 0; i < h; i++)
	{
		cin >> map[i];
		for (int j = 0; j < w; j++)
		{
			level[i][j] = 987654321;
		}
	}

	deque<pair<int, int> > que;

	que.push_back(make_pair(cx, cy));
	level[cx][cy] = 0;

	while (!que.empty())
	{
		pair<int, int> current = que.front();
		que.pop_front();

		if (visited[current.first][current.second])
		{
			continue;
		}
		visited[current.first][current.second] = true;

		// 5x5
		for (int i = -2; i <= 2; i++)
		{
			for (int j = -2; j <= 2; j++)
			{
				int x = current.first + i;
				int y = current.second + j;

				if (x < 0 || x >= h || y < 0 || y >= w || map[x][y] != '.' || visited[x][y])
				{
					continue;
				}

				que.push_back(make_pair(x, y));
				level[x][y] = min(level[x][y], level[current.first][current.second] + 1);
			}
		}

		// 4x4
		for (int d = 0; d < 4; d++)
		{
			int x = current.first + direction[d][0];
			int y = current.second + direction[d][1];

			if (x < 0 || x >= h || y < 0 || y >= w || map[x][y] != '.' || visited[x][y])
			{
				continue;
			}

			que.push_front(make_pair(x, y));
			level[x][y] = level[current.first][current.second];
		}
	}

	if (level[dx][dy] == 987654321)
	{
		cout << -1 << endl;
	}
	else
	{
		cout << level[dx][dy] << endl;
	}
}