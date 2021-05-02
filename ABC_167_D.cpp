#include <iostream>
#include <cstring>
using namespace std;

int a[200001];
int cnt[200001];

int main() {
	int n;
	long long k;
	cin >> n >> k;

	for (int i = 1; i <= n; i++)
	{
		cin >> a[i];
	}

	int turn = 0;
	int current = 1;
	int cycle = 1;

	while (k > 0)
	{
		if (cnt[current] > 0)
		{
			cycle = turn - cnt[current] + 1;
			k %= cycle;

			memset(cnt, 0, sizeof(cnt));
		}
		else
		{
			cnt[current] = ++turn;
			current = a[current];
			k--;
		}
	}

	cout << current << endl;
}