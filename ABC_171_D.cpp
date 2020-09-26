#include <iostream>
#include <map>
using namespace std;

int main() {
	int n, q;
	map<int, int> cnt;
	long long total = 0;

	cin >> n;

	for (int i = 0; i < n; i++)
	{
		int a;
		cin >> a;
		cnt[a] ++;
		total += a;
	}

	cin >> q;

	while (q--)
	{
		int b, c;
		cin >> b >> c;

		// b ������ŭ �� ���� c ������ŭ ���ϸ� ��.
		int bcnt = cnt[b];

		total -= (long long)(bcnt)*b;
		total += (long long)(bcnt)*c;

		cnt[b] -= bcnt;
		cnt[c] += bcnt;

		cout << total << endl;
	}
}