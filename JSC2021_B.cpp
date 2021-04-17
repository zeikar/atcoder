#include <iostream>
#include <map>
using namespace std;

int a[1000], b[1000];


int main() {
	int n, m;
	cin >> n >> m;

	map<int, int> cnt;

	for (int i = 0; i < n; i++)
	{
		cin >> a[i];
		cnt[a[i]]++;
	}
	for (int i = 0; i < m; i++)
	{
		cin >> b[i];
		cnt[b[i]]++;
	}

	for (int i = 1; i <= 1000; i++)
	{
		if (cnt[i] == 1)
		{
			cout << i << ' ';
		}
	}
	cout << endl;
}