#include <iostream>
using namespace std;

int main() {
	int n, k;
	cin >> n >> k;

	int ans = 0;
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= k; j++)
		{
			ans += 100 * i + j;
		}
	}

	cout << ans << endl;
}