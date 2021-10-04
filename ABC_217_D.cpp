#include <iostream>
#include <map>
using namespace std;

map<int, int>::const_iterator find(const map<int, int>& items, int value)
{
	auto itlow = items.lower_bound(value);

	if (itlow->first == value)
		return itlow;
	else if (itlow != items.cbegin())
		return --itlow;
	else
		return items.cend();
}

int main() {
	int l, q;

	cin >> l >> q;

	map<int, int> m;

	m[1] = 0;
	m[l] = l;

	while (q--)
	{
		int c, x;
		cin >> c >> x;

		if (c == 1)
		{
			m[x] = x;
		}
		else
		{
			auto rit = m.lower_bound(x);
			auto lit = find(m, x);

			cout << (rit->second) - (lit->second) << endl;
		}
	}
}