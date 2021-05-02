#include <iostream>
#include <cmath>
using namespace std;

int n;

long long f(int x) {
	long long m = n / x;

	return m * (m + 1) / 2 * x;
}

int main() {
	long long result = 0;
	cin >> n;

	for (int i = 1; i <= n; i++)
	{
		result += f(i);
	}

	cout << result << endl;
}