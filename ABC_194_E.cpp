#include <iostream>
#include <map>
using namespace std;

int a[1500000];
int cnt[1500001];

int main() {
	int n, m;
	cin >> n >> m;

	for (int i = 0; i < n; i++)
	{
		cin >> a[i];
	}

	map<int, bool> notOccurred;
	for (int i = 0; i <= n; i++)
	{
		notOccurred[i] = true;
	}

	for (int i = 0; i < m; i++)
	{
		if (cnt[a[i]] == 0)
		{
			notOccurred.erase(a[i]);
		}

		cnt[a[i]]++;
	}

	int ans = notOccurred.begin()->first;
	for (int i = m; i < n; i++)
	{
		if (cnt[a[i]] == 0)
		{
			notOccurred.erase(a[i]);
		}

		cnt[a[i]]++;	
		cnt[a[i - m]]--;

		if (cnt[a[i - m]] == 0)
		{
			notOccurred[a[i - m]] = true;
		}

		ans = min(ans, notOccurred.begin()->first);
	}

	cout << ans << endl;
}