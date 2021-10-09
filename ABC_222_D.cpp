#include <iostream>
#include <cstring>
using namespace std;

int n;
int a[3000];
int b[3000];
long long dp[3000][3001];
long long prefixSum[3001][3001];

long long solve(int idx, int prev) {
	if (idx == n)
	{
		return 1;
	}
	if (dp[idx][prev] != -1)
	{
		return dp[idx][prev];
	}

	if (prefixSum[idx][b[idx]] == 0)
	{
		for (int i = a[idx]; i <= b[idx]; i++)
		{
			prefixSum[idx][i] = (prefixSum[idx][i - 1] + solve(idx + 1, i)) % 998244353;
		}
	}

	long long ret = (prefixSum[idx][b[idx]] + 998244353 - prefixSum[idx][max(a[idx], prev) - 1] + 998244353) % 998244353;
	return dp[idx][prev] = ret;
}

int main() {
	memset(dp, -1, sizeof(dp));

	cin >> n;

	for (int i = 0; i < n; i++)
	{
		cin >> a[i];
	}

	for (int i = 0; i < n; i++)
	{
		cin >> b[i];
	}

	cout << solve(0, 0) << endl;
}