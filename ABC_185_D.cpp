#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

int a[200002];

int main() {
	int n, m;
	cin >> n >> m;

	vector<int> slices;

	int k = n, start = 0;
	for (int i = 1; i <= m; i++)
	{
		cin >> a[i];
	}
	a[m + 1] = n + 1;
	m += 2;

	sort(a, a + m);

	for (int i = 1; i < m; i++)
	{
		int slice = a[i] - start - 1;
		start = a[i];

		if (slice == 0)
		{
			continue;
		}

		slices.push_back(slice);
		k = min(k, slice);
	}

	long long res = 0;
	for (int i = 0; i < slices.size(); i++)
	{
		res += ceill((long double)slices[i] / k);
	}
	cout << res << endl;
}