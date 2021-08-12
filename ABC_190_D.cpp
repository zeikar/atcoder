#include <iostream>
using namespace std;

int main() {
	long long n;
	cin >> n;

	long long res = 0;

	// https://cp-wiki.vercel.app/en/tutorial/atcoder/ABC190/#problem-d-staircase-sequences
	for (long long i = 1; i * i <= 2 * n; i++)
	{
		if ((2 * n) % i == 0) {
			if (((2 * n / i) + 1 - i) % 2 == 0) {
				res += 2;
			}
		}
	}

	cout << res << endl;
}