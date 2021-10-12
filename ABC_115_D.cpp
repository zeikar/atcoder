#include <iostream>
using namespace std;

long long totalLength[51];
long long pattyLength[51];

void getLength(int level) {
	if (level == 0)
	{
		totalLength[level] = 1;
		pattyLength[level] = 1;
		return;
	}

	getLength(level - 1);

	totalLength[level] = totalLength[level - 1] * 2 + 3;
	pattyLength[level] = pattyLength[level - 1] * 2 + 1;
}

long long solve(int level, long long x) {
	if (level == 0)
	{
		return 1;
	}

	if (x <= 1)
	{
		return 0;
	}
	else if (x <= 1 + totalLength[level - 1])
	{
		return solve(level - 1, x - 1);
	}
	else if (x == 1 + totalLength[level - 1] + 1)
	{
		return pattyLength[level - 1] + 1;
	}
	else if (x < totalLength[level])
	{
		return pattyLength[level - 1] + 1 + solve(level - 1, x - totalLength[level - 1] - 2);
	}
	else
	{
		return pattyLength[level];
	}
}

int main() {
	int n;
	long long x;

	cin >> n >> x;

	getLength(n);

	cout << solve(n, x) << endl;
}