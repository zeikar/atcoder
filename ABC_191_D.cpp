#include <iostream>
#include <cmath>
using namespace std;

int main() {
	long double ox, oy, r;

	cin >> ox >> oy >> r;

	r += 1e-14;
	int start = ceil(oy - r);
	int end = floor(oy + r);

	long long res = 0;

	for (int i = start; i <= end; i++)
	{
		// x^2 + y^2 == r^2
		long double y = i - oy;
		long double sq = sqrtl(r * r - y * y);

		long double x1 = ceil(-sq + ox);
		long double x2 = floor(sq + ox);

		res += x2 - x1 + 1;
	}

	cout << res << endl;
}