#include <iostream>
#include <cmath>
using namespace std;

int main() {
	long long a, b, c;

	cin >> a >> b >> c;

	if (c % 2 == 0)
	{
		if (abs(a) > abs(b))
		{
			cout << ">" << endl;
		}
		else if (abs(a) == abs(b))
		{
			cout << "=" << endl;
		}
		else
		{
			cout << "<" << endl;
		}
	}
	else
	{
		if (a < 0 && b < 0)
		{
			if (abs(a) > abs(b))
			{
				cout << "<" << endl;
			}
			else if (abs(a) == abs(b))
			{
				cout << "=" << endl;
			}
			else
			{
				cout << ">" << endl;
			}
		}
		else if (a < 0)
		{
			cout << "<" << endl;
		}
		else if (b < 0)
		{
			cout << ">" << endl;
		}
		else
		{
			if (abs(a) > abs(b))
			{
				cout << ">" << endl;
			}
			else if (abs(a) == abs(b))
			{
				cout << "=" << endl;
			}
			else
			{
				cout << "<" << endl;
			}
		}
	}
}