#include <iostream>
#include <algorithm>
using namespace std;

long long a[100000];

int main() {
	int n;
	cin >> n;

	for (int i = 0; i < n; i++)
	{
		cin >> a[i];
	}

	sort(a, a + n);

	long long ans = 0;

	if (n % 2 == 0)
	{
		for (int i = 0; i < n / 2; i++)
		{
			ans -= 2 * a[i];
		}
		for (int i = n / 2; i < n; i++)
		{
			ans += 2 * a[i];
		}
		ans -= a[n / 2];
		ans += a[n / 2 - 1];
	}
	else
	{
		long long oddBig = 0, evenBig = 0;

		for (int i = 0; i < n / 2; i++)
		{
			oddBig -= 2 * a[i];
		}
		for (int i = n / 2; i < n; i++)
		{
			oddBig += 2 * a[i];
		}
		oddBig -= a[n / 2];
		oddBig -= a[n / 2 + 1];


		for (int i = 0; i <= n / 2; i++)
		{
			evenBig -= 2 * a[i];
		}
		for (int i = n / 2 + 1; i < n; i++)
		{
			evenBig += 2 * a[i];
		}
		evenBig += a[n / 2];
		evenBig += a[n / 2 - 1];

		ans = max(oddBig, evenBig);
	}


	cout << ans << endl;
}