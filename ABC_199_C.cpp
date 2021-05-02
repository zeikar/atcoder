#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

int main() {
	int n;
	string str;
	int q;
	int swap = 0;

	cin >> n >> str >> q;

	while (q--)
	{
		int t, a, b;

		cin >> t >> a >> b;

		if (t == 1)
		{
			a--;
			b--;

			if (swap % 2 == 1)
			{
				if (a >= n)
				{
					a -= n;
				}
				else
				{
					a += n;
				}
				if (b >= n)
				{
					b -= n;
				}
				else
				{
					b += n;
				}
			}

			char temp = str[a];
			str[a] = str[b];
			str[b] = temp;
		}
		else
		{
			swap++;
		}
	}

	if (swap % 2 == 1)
	{
		cout << str.substr(n) + str.substr(0, n);
	}
	else
	{
		cout << str << endl;
	}

}