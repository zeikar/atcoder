#include <iostream>
#include <algorithm>
using namespace std;

int a[200000];

int main() {
	int n, q;
	cin >> n >> q;

	for (int i = 0; i < n; i++)
	{
		cin >> a[i];
	}

	sort(a, a + n);

	while (q--)
	{
		int x;
		cin >> x;

		auto cnt = n - (lower_bound(a, a + n, x) - a);

		cout << cnt << endl;
	}
}