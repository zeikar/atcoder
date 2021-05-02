#include <iostream>
#include <string>
using namespace std;

int main() {
	string s;
	cin >> s;

	int res = 0;
	for (int i = 0; i < s.length(); i++)
	{
		if (s.substr(i, 4) == "ZONe")
		{
			res++;
		}
	}

	cout << res << endl;
}