#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <cmath>
using namespace std;

long long dp[200001];
long long prefix[200001];
const int mod = 998244353;

int main()
{
	int n, k;
	cin >> n >> k;

	vector<pair<int, int> > set;
	for (int i = 0; i < k; i++)
	{
		int l, r;
		cin >> l >> r;
		set.push_back(make_pair(l, r));
	}
	sort(set.begin(), set.end());

	// bottom up dp
	dp[1] = 1;
	prefix[1] = 1;
	for (int i = 1; i <= n; i++)
	{
		for (int j = 0; j < k; j++)
		{
			int left = set[j].first;
			int right = set[j].second;

			int start = max(i - right - 1, 0);
			int end = max(i - left, 0);

			// (i - right) ~ (i - left) 까지의 합
			dp[i] += prefix[end] - prefix[start] + mod;
			dp[i] %= mod;
		}

		prefix[i] = (prefix[i - 1] + dp[i]) % mod;
	}

	cout << dp[n] << endl;
}