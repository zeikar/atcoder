#include <iostream>
using namespace std;

int main() {
	int x, y, z;
	cin >> x >> y >> z;

	long double g = (long double)y / x;
	int answer = 0;
	for (int i = 0; i < 10000000; i++)
	{
		long double a = (long double)i / z;

		if (a >= g)
		{
			answer = i - 1;
			break;
		}
	}

	cout << answer << endl;
}