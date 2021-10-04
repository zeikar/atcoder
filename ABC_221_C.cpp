#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
using namespace std;

int strtoi(string s, int idx, int cnt) {
	int ret = 0;
	for (int i = 0; i < cnt; i++)
	{
		ret *= 10;
		ret += s[idx + i] - '0';
	}
	return ret;
}

int main() {
	string n;

	cin >> n;

	sort(n.begin(), n.end());

	int ans = 0;
	
	do {

		for (int k = 0; k < n.length(); k++)
		{
			int a = 0, b = 0;

			for (int i = 0; i < n.length(); i++)
			{
				if (i < k)
				{
					a *= 10;
					a += n[i] - '0';
				}
				else
				{
					b *= 10;
					b += n[i] - '0';
				}
			}

			ans = max(ans, a * b);
		}


	} while (std::next_permutation(n.begin(), n.end()));


	cout << ans << endl;
}