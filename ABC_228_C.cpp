#include <iostream>
#include <algorithm>
using namespace std;

int p[100001];
int sorted[100001];

int main() {
	int n, k;
	cin >> n >> k;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int t;
			cin >> t;
			p[i] += t;
		}

		sorted[i] = p[i];
	}

	sort(sorted, sorted + n);

	for (int i = 0; i < n; i++)
	{
		int rank = n - (upper_bound(sorted, sorted + n, p[i] + 300) - sorted) + 1;

		if (rank <= k)
		{
			cout << "Yes" << endl;
		}
		else
		{
			cout << "No" << endl;
		}
	}
}