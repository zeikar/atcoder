#include <iostream>
using namespace std;

int main() {
	long long a, b, n;

	cin >> a >> b >> n;

	long long x = min(b - 1, n);

	cout << ((a * x) / b) - (a * (x / b)) << endl;
}