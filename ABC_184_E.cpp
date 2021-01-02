#include <iostream>
#include <queue>
#include <utility>
#include <string>
#include <cstring>
using namespace std;

string maps[2000];
int visited[2000][2000];
bool teleported[26];

vector<pair<int, int> > teleport[26];
pair<int, int> start, goal;

int dir[][2] = { -1, 0, 0, -1, 0, 1,1,0 };

int main() {
	int h, w;

	cin >> h >> w;

	for (int i = 0; i < h; i++)
	{
		cin >> maps[i];

		for (int j = 0; j < maps[i].size(); j++)
		{
			if (maps[i][j] >= 'a' && maps[i][j] <= 'z')
			{
				teleport[maps[i][j] - 'a'].push_back(make_pair(i, j));
			}
			else if (maps[i][j] == 'S')
			{
				start = make_pair(i, j);
			}
			else if (maps[i][j] == 'G')
			{
				goal = make_pair(i, j);
			}
		}
	}

	memset(visited, -1, sizeof(visited));

	int result = -1;

	queue< pair<int, int> > que;
	que.push(start);
	visited[start.first][start.second] = 0;

	while (!que.empty())
	{
		pair<int, int> next = que.front();
		que.pop();

		if (next.first == goal.first && next.second == goal.second)
		{
			result = visited[next.first][next.second];
			break;
		}

		for (int d = 0; d < 4; d++)
		{
			int x = next.first + dir[d][0];
			int y = next.second + dir[d][1];

			if (x >= h || x < 0 || y >= w || y < 0 || visited[x][y] != -1 || maps[x][y] == '#')
			{
				continue;
			}

			visited[x][y] = visited[next.first][next.second] + 1;
			que.push(make_pair(x, y));
		}

		// teleport
		if (maps[next.first][next.second] >= 'a' && maps[next.first][next.second] <= 'z' && teleported[maps[next.first][next.second] - 'a'] == false)
		{
			teleported[maps[next.first][next.second] - 'a'] = true;

			for (int i = 0; i < teleport[maps[next.first][next.second] - 'a'].size(); i++)
			{
				int x = teleport[maps[next.first][next.second] - 'a'][i].first;
				int y = teleport[maps[next.first][next.second] - 'a'][i].second;

				if (x >= h || x < 0 || y >= w || y < 0 || visited[x][y] != -1 || maps[x][y] == '#')
				{
					continue;
				}

				visited[x][y] = visited[next.first][next.second] + 1;
				que.push(make_pair(x, y));
			}
		}
	}

	cout << result << endl;
}