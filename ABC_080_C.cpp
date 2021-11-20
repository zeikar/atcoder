#include <iostream>
using namespace std;

int f[100][10];
int p[100][11];

int main() {
	int n;
	cin >> n;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			cin >> f[i][j];
		}
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			cin >> p[i][j];
		}
	}

	int ans = -1987654321;
	for (int i = 1; i < (1 << 10); i++)
	{
		int profit = 0;

		for (int j = 0; j < n; j++)
		{
			int cnt = 0;

			for (int k = 0; k < 10; k++)
			{
				// open
				if (i & (1 << k) && f[j][k] == 1)
				{
					cnt++;
				}
			}

			profit += p[j][cnt];
		}

		ans = max(ans, profit);
	}

	cout << ans << endl;
}