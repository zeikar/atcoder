#include <iostream>
using namespace std;

long long a[100000];

int main() {
	long long n, k;
	cin >> n >> k;

	long long high = 0;
	long long low = 0;
	long long ans = 0;

	for (int i = 0; i < n; i++)
	{
		cin >> a[i];

		high = max(high, a[i]);
	}

	while (low <= high)
	{
		long long mid = (high + low) / 2;
		long long cnt = 0;

		for (int i = 0; i < n; i++)
		{
			if (a[i] < mid)
			{
				continue;
			}

			long long n = a[i] - mid + 1;
			cnt += n;
		}

		if (cnt > k)
		{
			low = mid + 1;
		}
		else
		{
			high = mid - 1;
		}
	}

	long long cnt = 0;
	for (int i = 0; i < n; i++)
	{
		if (a[i] < low)
		{
			continue;
		}

		long long n = a[i] - low + 1;
		cnt += n;
		ans += n * (a[i] + low) / 2;
	}
	ans += max((k - cnt) * (low - 1), 0LL);

	cout << ans << endl;
}