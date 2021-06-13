#include <iostream>
using namespace std;

int main() {
	int n;
	long long result = 0;
	cin >> n;

	for (int i = 0; i < n; i++)
	{
		int a;
		cin >> a;

		result += max(0, a - 10);
	}

	cout << result << endl;
}