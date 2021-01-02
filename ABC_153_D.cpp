#include <iostream>
#include <map>
using namespace std;

map<long long, long long> dp;

long long solve(long long n) {
	if (n == 1)
	{
		return 1;
	}
	if (dp[n] != 0)
	{
		return dp[n];
	}

	return dp[n] = solve(n / 2) + solve(n / 2) + 1;
}

int main() {
	long long h;

	cin >> h;

	cout << solve(h) << endl;
}