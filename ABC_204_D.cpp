#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;

int n;
int t[100];
int oven[2];
int dp[100000][100];

int go(int oven0, int oven1, int dishidx) {
	if (dishidx >= n)
	{
		return max(oven0, oven1);
	}
	if (dp[oven0][dishidx] != -1)
	{
		return dp[oven0][dishidx];
	}

	// oven0
	int ret = 987654321;
	ret = min(ret, go(oven0 + t[dishidx], oven1, dishidx + 1));

	// oven1
	ret = min(ret, go(oven0, oven1 + t[dishidx], dishidx + 1));

	return dp[oven0][dishidx]= ret;
}

int main() {
	cin >> n;

	for (int i = 0; i < n; i++)
	{
		cin >> t[i];
	}

	memset(dp, -1, sizeof(dp));
	cout << go(0, 0, 0) << endl;
}