#include <iostream>
#include <algorithm>
using namespace std;

int a[200000];

int main() {
	int n;
	cin >> n;

	for (int i = 0; i < n; i++)
	{
		cin >> a[i];

		a[i] -= (i + 1);
	}

	sort(a, a + n);

	int median = a[(n - 1) / 2];

	long long ans = 0;
	for (int i = 0; i < n; i++)
	{
		ans += abs(a[i] - median);
	}

	cout << ans << endl;
}