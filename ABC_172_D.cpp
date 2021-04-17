#include <iostream>
#include <vector>
using namespace std;

vector<int> primes;

int f(int x) {
	if (x == 1)
	{
		return 1;
	}

	for (int i = 0; i < primes.size(); i++)
	{
		if (primes[i] > x)
		{
			return 1;
		}

		if (primes[i] * primes[i] == x)
		{
			return f(x / primes[i]) + 1;
		}
		if (x % primes[i] == 0)
		{
			return f(x / primes[i]) + 2;
		}
	}

	return 1;
}

int main() {
	int n;
	long long result = 0;
	cin >> n;

	// primes
	for (int i = 2; i <= 1000000; i++)
	{
		bool ok = true;
		for (int j = 2; j * j <= i; j++)
		{
			if (i % j == 0)
			{
				ok = false;
				break;
			}
		}

		if (ok)
		{
			primes.push_back(i);
		}
	}

	for (int i = 1; i <= n; i++)
	{
		result += i * f(i);

		cout << i << ' ' << f(i) << endl;
	}

	cout << result << endl;
}