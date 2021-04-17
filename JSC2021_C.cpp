#include <iostream>
#include <cmath>
using namespace std;

int gcd(int a, int b) {
	if (b == 0)
		return a;
	return gcd(b, a % b);
}

int main() {
	int a, b;
	cin >> a >> b;

	int ans = 0;

	for (int i = 1; i < 10000000; i++)
	{
		int ai = ceil((double)a / i);
		int bi = b / i;

		if (a <= i * ai && i * bi <= b && ai < bi)
		{
			ans = max(ans, i);
		}
	}

	cout << ans << endl;
}