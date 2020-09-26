#include <iostream>
#include <cstring>
using namespace std;

const int mod = 1e9 + 7;
int dp[2001];
int s;

int solve(int n) {
	int ret = 0;

	if (n == 0)
	{
		return 1;
	}
	if (dp[n] != -1)
	{
		return dp[n];
	}

	for (int i = 3; i <= n; i++)
	{
		ret += solve(n - i);
		ret %= mod;
	}

	return dp[n] = ret;
}

int main() {
	memset(dp, -1, sizeof(dp));

	cin >> s;

	cout << solve(s) << endl;
}