#include <iostream>
#include <string>
#include <algorithm>
#include <map>
using namespace std;

int main() {
	string s;
	int k;

	cin >> s >> k;

	map<string, bool> substrs;
	for (int i = 0; i < s.length(); i++)
	{
		for (int j = i + 1; j <= min(i + k, (int)s.length()); j++)
		{
			substrs[s.substr(i, j - i)] = true;

			if (substrs.size() > k)
			{
				substrs.erase(--substrs.end());
			}
		}
	}

	for (auto const& str : substrs)
	{
		k--;

		if (k == 0)
		{
			cout << str.first << endl;
			break;
		}
	}
}