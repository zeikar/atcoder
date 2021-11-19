#include <iostream>
using namespace std;

int main() {
	int n, k;

	cin >> n >> k;

	long long res = 0;
	for (int b = k + 1; b <= n; b++)
	{
		// k to b-1
		int cnt = b - k;
		res += cnt * (n / b);

		res += max(0, n % b - k + 1);
	}

	if (k == 0)
	{
		res -= n;
	}

	cout << res << endl;
}