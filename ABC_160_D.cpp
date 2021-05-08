#include <iostream>
using namespace std;

int cnt[2001];

int main() {
	int n, x, y;
	cin >> n >> x >> y;

	for (int i = 1; i <= n; i++)
	{
		for (int j = i + 1; j <= n; j++)
		{
			if (j < x || i > y)
			{
				cnt[j - i]++;
			}
			else if (i <= x && j >= y)
			{
				cnt[j - i - (y - x) + 1]++;
			}
			// x-y
			else
			{
				int left = abs(i - x);
				if (i <= x)
				{
					left = min(left, x - i);
				}
				int right = abs(y - j);
				if (j >= y)
				{
					right = min(right, j - y);
				}
				int b = j - i;

				cnt[min(left + right + 1, b)]++;
			}
		}
	}

	for (int i = 1; i < n; i++)
	{
		cout << cnt[i] << endl;
	}
}