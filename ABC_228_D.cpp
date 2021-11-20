#include <iostream>
#include <cstring>
#include <map>
#include <algorithm>
using namespace std;

long long a[(1 << 20)];
map<long long, int> fit;

int main() {
	int n = (1 << 20);
	memset(a, -1, sizeof(a));

	for (int i = 0; i < 2 * n; i++)
	{
		fit[i] = true;
	}

	int q;
	cin >> q;
	while (q--)
	{
		int t;
		long long x;
		cin >> t >> x;

		if (t == 1)
		{
			long long idx = x % n;
			auto upper = fit.lower_bound(idx);

			idx = upper->first % n;

			a[idx] = x;
			fit.erase(idx);
			fit.erase(idx + n);
		}
		else
		{
			cout << a[x % n] << endl;
		}
	}
}