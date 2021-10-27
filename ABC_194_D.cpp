#include <iostream>
#include <iomanip>
using namespace std;

int main() {
	int n;
	cin >> n;

	long double ans = 0;
	for (int i = 1; i < n; i++)
	{
		ans += (long double)n / (n - i);
	}

	cout << fixed << setprecision(12) << ans << endl;
}