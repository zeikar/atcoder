#include <iostream>
using namespace std;

int main() {
	long long x;
	cin >> x;

	for (int a = -1000; a < 1000; a++)
	{
		for (int b = -1000; b < 1000; b++)
		{
			long long ans = (long long)a * a * a * a * a - (long long)b * b * b * b * b;

			if (ans == x)
			{
				cout << a << ' ' << b << endl;
				return 0;
			}
		}
	}
}