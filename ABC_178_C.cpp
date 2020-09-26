#include <iostream>
using namespace std;

// [i][0] => 0.9 �Ѵ� ����
// [i][1] => 0�� ����
// [i][2] => 9�� ����
// [i][3] => 0,9 �Ѵ� ����
long long dp[1000001][4];

int main() {
	const int mod = 1e9 + 7;
	int n;
	cin >> n;

	if (n == 1)
	{
		cout << 0 << endl;
		return 0;
	}

	long long ans = 2;

	dp[1][0] = 8;
	dp[1][1] = 1;
	dp[1][2] = 1;

	for (int i = 2; i <= n; i++)
	{
		// ai �� 1-8
		dp[i][0] += dp[i - 1][0] * 8;
		dp[i][0] %= mod;
		dp[i][1] += dp[i - 1][1] * 8;
		dp[i][1] %= mod;
		dp[i][2] += dp[i - 1][2] * 8;
		dp[i][2] %= mod;
		dp[i][3] += dp[i - 1][3] * 8;
		dp[i][3] %= mod;
		// ai �� 0
		dp[i][1] += dp[i - 1][1] + dp[i - 1][0];
		dp[i][1] %= mod;
		dp[i][3] += dp[i - 1][3] + dp[i - 1][2];
		dp[i][3] %= mod;

		// ai �� 9
		dp[i][2] += dp[i - 1][2] + dp[i - 1][0];
		dp[i][2] %= mod;
		dp[i][3] += dp[i - 1][3] + dp[i - 1][1];
		dp[i][3] %= mod;
	}

	cout << dp[n][3] << endl;
}