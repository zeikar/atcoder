#include <iostream>
#include <cstring>
using namespace std;

int dp[100001];

int main() {
	int N;
	cin >> N;

	for (int i = 1; i <= N; i++)
	{
		dp[i] = 987654321;

		for (int j = 6; j <= i; j *= 6)
		{
			dp[i] = min(dp[i], dp[i - j] + 1);
		}
		for (int j = 9; j <= i; j *= 9)
		{
			dp[i] = min(dp[i], dp[i - j] + 1);
		}

		dp[i] = min(dp[i], dp[i - 1] + 1);
	}

	cout << dp[N] << endl;
}