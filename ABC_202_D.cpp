#include <iostream>
#include <string>
using namespace std;

long long C[61][61];

// Returns value of Binomial Coefficient C(n, k)
long long binomialCoeff(int n, int k)
{
	int i, j;

	// Caculate value of Binomial Coefficient
	// in bottom up manner
	for (i = 0; i <= n; i++) {
		for (j = 0; j <= min(i, k); j++) {
			// Base Cases
			if (j == 0 || j == i)
				C[i][j] = 1;

			// Calculate value using previously
			// stored values
			else
				C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
		}
	}

	return C[n][k];
}

string go(int a, int b, long long k) {
	if (k <= 0 || a <= 0 || b <= 0)
	{
		string ret = "";

		while (a--)
		{
			ret += 'a';
		}
		while (b--)
		{
			ret += 'b';
		}

		return ret;
	}

	// a
	long long c = binomialCoeff(a + b - 1, a - 1);
	if (k <= c)
	{
		return 'a' + go(a - 1, b, k);
	}
	else
	{
		return 'b' + go(a, b - 1, k - c);
	}
}

int main() {
	long long a, b, k;
	cin >> a >> b >> k;

	cout << go(a, b, k) << endl;
}