#include <iostream>
#include <string>
using namespace std;

int main() {
	string s;
	cin >> s;

	long long ans = 0;

	for (int i = 0; i < (1 << (s.length() - 1)); i++)
	{
		long long num = 0;

		for (int j = 0; j < s.length(); j++)
		{
			num *= 10;
			num += s[j] - '0';

			if ((1 << j) & i)
			{
				ans += num;
				num = 0;
			}
		}

		ans += num;
	}

	cout << ans << endl;
}