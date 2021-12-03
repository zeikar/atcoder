#include <iostream>
#include <string>
using namespace std;

int main() {
	string s;
	cin >> s;

	string oxx = "oxx";

	for (int i = 0; i < 3; i++)
	{
		bool ok = true;

		for (int j = 0; j < s.length(); j++)
		{
			if (oxx[(j + i) % 3] != s[j])
			{
				ok = false;
				break;
			}
		}

		if (ok)
		{
			cout << "Yes" << endl;
			return 0;
		}
	}

	cout << "No" << endl;
}