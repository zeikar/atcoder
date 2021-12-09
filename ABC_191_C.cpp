#include <iostream>
#include <string>
using namespace std;

int main() {
	int h, w;
	cin >> h >> w;

	string grid[10];

	for (int i = 0; i < h; i++)
	{
		cin >> grid[i];
	}

	int ans = 0;

	for (int i = 0; i < h - 1; i++)
	{
		for (int j = 0; j < w - 1; j++)
		{
			int cnt = 0;

			for (int a = 0; a < 2; a++)
			{
				for (int b = 0; b < 2; b++)
				{
					cnt += (grid[i + a][j + b] == '#');
				}
			}

			if (cnt == 1 || cnt == 3)
			{
				ans++;
			}
		}
	}

	cout << ans << endl;
}