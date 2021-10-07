#include <iostream>
#include <string>
using namespace std;

int main() {
	string s;
	cin >> s;

	// dummy
	s += "F";

	long long ans = 0;
	long long aCount = 0;

	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] == 'A')
		{
			aCount++;
		}
		else if (s.substr(i, 2) == "BC")
		{
			ans += aCount;
			i++;
		}
		else
		{
			aCount = 0;
		}
	}

	cout << ans << endl;
}