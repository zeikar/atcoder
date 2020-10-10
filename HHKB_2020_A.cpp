#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>
using namespace std;

int main() {
	string s, t;
	cin >> s >> t;

	if (s == "Y")
	{
		cout << (char)(t[0] - 'a'+'A') << endl;
	}
	else
	{
		cout << t << endl;
	}
}