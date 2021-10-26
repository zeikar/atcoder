#include <iostream>
#include <map>
using namespace std;

int main() {
	int n;
	cin >> n;

	map<int, int> cnt;

	for (int i = 0; i < n; i++)
	{
		int a;
		cin >> a;
		cnt[a]++;
	}

	int toRemove = 0;
	for (auto& i : cnt)
	{
		if (i.second > 1)
		{
			toRemove += i.second - 1;
		}
	}

	if (toRemove % 2 == 0)
	{
		cout << cnt.size() << endl;
	}
	else
	{
		cout << cnt.size() - 1 << endl;
	}
}