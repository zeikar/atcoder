#include <iostream>
#include <string>
using namespace std;

int main() {
	long long n, a, b;
	long long sum = 0;

	cin >> n;

	while (n--)
	{
		cin >> a >> b;

		a--;
		sum += (b * (b + 1) / 2) - (a * (a + 1) / 2);
	}

	cout << sum << endl;
}