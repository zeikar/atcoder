#include <iostream>
#include <string>
using namespace std;

string grid[2000];
int consecutiveX[2000][2000];
int consecutiveY[2000][2000];

int main() {
	int H, W;

	cin >> H >> W;

	for (int i = 0; i < H; i++)
	{
		cin >> grid[i];
	}

	int ans = 0;
	int cnt = 0;

	for (int i = 0; i < H; i++)
	{
		int startX = i, startY = 0;

		for (int j = 0; j < W; j++)
		{
			if (grid[i][j] == '.')
			{
				++cnt;
			}
			else
			{
				for (int k = startY; k < j; k++)
				{
					consecutiveX[startX][k] = cnt;
				}

				startY = j + 1;
				cnt = 0;
			}
		}

		for (int k = startY; k < W; k++)
		{
			consecutiveX[startX][k] = cnt;
		}

		cnt = 0;
	}

	for (int j = 0; j < W; j++)
	{
		int startX = 0, startY = j;

		for (int i = 0; i < H; i++)
		{
			if (grid[i][j] == '.')
			{
				++cnt;
			}
			else
			{
				for (int k = startX; k < i; k++)
				{
					consecutiveY[k][startY] = cnt;
				}

				startX = i + 1;
				cnt = 0;
			}
		}

		for (int k = startX; k < H; k++)
		{
			consecutiveY[k][startY] = cnt;
		}

		cnt = 0;
	}

	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			ans = max(ans, consecutiveX[i][j] + consecutiveY[i][j] - 1);
		}
	}

	cout << ans << endl;
}