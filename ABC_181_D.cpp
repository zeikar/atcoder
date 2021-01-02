#include <iostream>
#include <string>
#include <map>
using namespace std;

int digits[10];

bool isAvailable(int n) {
	int temp[10];
	for (int i = 0; i < 10; i++)
	{
		temp[i] = digits[i];
	}

	int num = n;
	while (num != 0)
	{
		int d = num % 10;

		if (temp[d] > 0)
		{
			temp[d]--;
		}
		else
		{
			return false;
		}

		num /= 10;
	}

	if (n < 100)
	{
		for (int i = 0; i < 10; i++)
		{
			if (temp[i] != 0)
			{
				return false;
			}
		}
	}

	return true;
}

int main() {
	string s;
	cin >> s;

	for (int i = 0; i < s.length(); i++)
	{
		digits[s[i] - '0']++;
	}

	int num = 0;
	for (int i = 8; i < 1000; i += 8)
	{
		if (isAvailable(i))
		{
			cout << "Yes" << endl;
			return 0;
		}
	}
	cout << "No" << endl;
}