#include <iostream>
#include <string>
using namespace std;

int main() {
	int n, k;
	string s;

	cin >> n >> k >> s;

	int happy = 0;
	for (int i = 1; i < n; i++)
	{
		if (s[i] == s[i - 1])
		{
			happy++;
		}
	}

	cout << min(n - 1, happy + 2 * k) << endl;
}