#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

int main() {
	string s, t;
	cin >> s >> t;

	string ans = "Yes";

	int cnt = 0;

	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] == t[i])
		{
			continue;
		}

		if (cnt == 1)
		{
			ans = "No";
			break;
		}

		if (i + 1 < s.length())
		{
			cnt = 1;

			char temp = t[i];
			t[i] = t[i + 1];
			t[i + 1] = temp;
			i--;
		}
		else
		{
			ans = "No";
			break;
		}
	}

	cout << ans << endl;
}