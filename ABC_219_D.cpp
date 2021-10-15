#include <iostream>
#include <cstring>
using namespace std;

int n, x, y;
int a[300], b[300];
int dp[301][301][301];

int solve(int idx, int cx, int cy) {
	if (cx >= x && cy >= y)
	{
		return 0;
	}
	if (idx >= n)
	{
		return 987654321;
	}
	if (dp[idx][min(cx, x)][min(cy, y)] != -1)
	{
		return dp[idx][min(cx, x)][min(cy, y)];
	}

	int ret = 987654321;
	// pick
	ret = min(ret, solve(idx + 1, cx + a[idx], cy + b[idx]) + 1);

	// not pick
	ret = min(ret, solve(idx + 1, cx, cy));

	return dp[idx][min(cx, x)][min(cy, y)] = ret;
}

int main() {
	memset(dp, -1, sizeof(dp));

	cin >> n >> x >> y;

	for (int i = 0; i < n; i++)
	{
		cin >> a[i] >> b[i];
	}

	int ans = solve(0, 0, 0);

	if (ans == 987654321)
	{
		ans = -1;
	}
	cout << ans << endl;
}