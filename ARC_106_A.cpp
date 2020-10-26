#include <iostream>
#include <algorithm>
#include <climits>

using namespace std;

unsigned long long ppow(unsigned long long num, unsigned long long m)
{
	if (m == 0)
	{
		return 1;
	}
	if (m % 2 == 0)
	{		
		return ppow(num, m / 2) * ppow(num, m / 2);
	}
	else
	{
		return ppow(num, m - 1) * num;
	}
}

unsigned long long ansa= -1, ansb=-1;
unsigned long long n;

void solve() {
	for (unsigned long long a = 1; ppow(3, a) <= n; a++)
	{
		for (unsigned long long  b = 1; ppow(5, b) <= n; b++)
		{
			if (ppow(3, a) + ppow(5, b) == n)
			{
				ansa = a;
				ansb = b;
				return;
			}
		}
	}
}

int main() {
	cin >> n;

	solve();

	if (ansa == -1 && ansb == -1)
	{
		cout << "-1" << endl;
	}
	else
	{
		cout << ansa << ' ' << ansb << endl;
	}
}