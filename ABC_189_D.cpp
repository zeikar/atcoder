#include <iostream>
#include <string>
#include <cstring>
using namespace std;

int n;
string expp[60];
long long dp[60][2];

long long go(bool current, int idx) {
	if (idx == n)
	{
		if (current == false)
		{
			return 0;
		}
		return 1;
	}
	if (dp[idx][current ? 0 : 1] != -1)
	{
		return dp[idx][current ? 0 : 1];
	}

	long long ret = 0;

	if (expp[idx] == "OR")
	{
		ret += go(current || false, idx + 1);
		ret += go(current || true, idx + 1);
	}
	else
	{
		ret += go(current && false, idx + 1);
		ret += go(current && true, idx + 1);
	}

	return dp[idx][current ? 0 : 1] = ret;
}

int main() {
	cin >> n;

	for (int i = 0; i < n; i++)
	{
		cin >> expp[i];
	}

	memset(dp, -1LL, sizeof(dp));

	cout << go(true, 0) + go(false, 0) << endl;
}