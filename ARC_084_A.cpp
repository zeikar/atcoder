#include <iostream>
#include <algorithm>
using namespace std;

int a[100000];
int b[100000];
int c[100000];

int main() {
	int n;
	cin >> n;

	for (int i = 0; i < n; i++)
	{
		cin >> a[i];
	}

	for (int i = 0; i < n; i++)
	{
		cin >> b[i];
	}

	for (int i = 0; i < n; i++)
	{
		cin >> c[i];
	}

	sort(a, a + n);
	sort(b, b + n);
	sort(c, c + n);

	long long ans = 0;

	for (int i = 0; i < n; i++)
	{
		int aIdx = lower_bound(a, a + n, b[i]) - a - 1;
		int cIdx = upper_bound(c, c + n, b[i]) - c;

		if (aIdx < 0 || cIdx >= n)
		{
			continue;
		}

		ans += (long long)(aIdx + 1) * (n - cIdx);
	}

	cout << ans << endl;
}