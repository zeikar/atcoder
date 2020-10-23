// source code based on https://www.acmicpc.net/problem/2098

#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;

int dp[17][1 << 18];
int N;
int x[17];
int y[17];
int z[17];

int TSP(int start, int path)
{
	if (dp[start][path] != 0)
	{
		return dp[start][path];
	}

	if (path == 0)
	{
		return abs(x[start] - x[0]) + abs(y[start] - y[0]) + max(0, z[0] - z[start]);
	}

	int ret = 987654321;

	for (int i = 0; i < N; ++i)
	{
		if (((1 << i) & path) == 0 || (path ^ (1 << i)) == path)
		{
			continue;
		}

		ret = min(ret, TSP(i, path ^ (1 << i)) + abs(x[start] - x[i]) + abs(y[start] - y[i]) + max(0, z[i] - z[start]));
	}

	return dp[start][path] = ret;
}

int main()
{
	cin >> N;

	for (int i = 0; i < N; ++i)
	{
		cin >> x[i] >> y[i] >> z[i];
	}

	cout << TSP(0, (1 << N) - 2) << endl;
}