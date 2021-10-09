#include <iostream>
#include <string>
using namespace std;

int n;

void solve(string s, int prevMax) {
	if (s.length() == n)
	{
		cout << s << endl;
		return;
	}

	for (int i = 0; i <= prevMax + 1; i++)
	{
		solve(s + (char)('a' + i), max(prevMax, i));
	}
}

int main() {
	cin >> n;

	solve("", -1);
}