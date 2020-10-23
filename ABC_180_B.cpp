#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cmath>
using namespace std;

int main() {
	int n;
	long double a = 0, b = 0, c = 0;

	cin >> n;

	while (n--)
	{
		long double x;
		cin >> x;

		a += abs(x);
		b += x * x;
		c = max(c, abs(x));
	}

	cout << setprecision(12) << a << '\n' << sqrt(b) << '\n' << c << endl;
}