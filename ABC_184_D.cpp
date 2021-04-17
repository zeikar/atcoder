#include <iostream>
#include <iomanip>
using namespace std;

long double dp[100][100][100];

long double solve(int a, int b, int c, int cnt) {
	// basis
	if (a == 100 || b == 100 || c == 100)
	{
		return cnt;
	}

	if (dp[a][b][c] != 0)
	{
		return dp[a][b][c];
	}

	long double ret = 0;

	// a + 1
	ret += solve(a + 1, b, c, cnt + 1) * a / (a + b + c);

	// b + 1
	ret += solve(a, b + 1, c, cnt + 1) * b / (a + b + c);

	// c + 1
	ret += solve(a, b, c + 1, cnt + 1) * c / (a + b + c);

	return  dp[a][b][c] = ret;
}

int main() {
	int a, b, c;
	cin >> a >> b >> c;

	cout << setprecision(12) << solve(a, b, c, 0) << endl;
}