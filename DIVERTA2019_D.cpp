#include <iostream>
using namespace std;

int main() {
	long long n;
	cin >> n;

	long long ans = 0;

	for (long long x = 1; x * x <= n; x++)
	{
		long long m = (n - x) / x;

		if (m == 0)
		{
			continue;
		}

		if (n / m == n % m)
		{
			ans += m;
		}
	}

	cout << ans << endl;
}