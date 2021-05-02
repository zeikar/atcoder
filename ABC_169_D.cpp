#include <iostream>
#include <map>
#include <vector>
using namespace std;

vector<long long> divisors;
map<long long, int> divCnt;

int main() {
	long long n;
	cin >> n;

	long long x = n;
	for (long long i = 2; i * i <= n; i++)
	{
		while (x % i == 0)
		{
			if (divCnt[i] == 0)
			{
				divisors.push_back(i);
			}

			divCnt[i]++;

			x /= i;
		}
	}

	if (x != 1)
	{
		divisors.push_back(x);
		divCnt[x]++;
	}

	int res = 0;
	for (int i = 0; i < divisors.size(); i++)
	{
		int cnt = divCnt[divisors[i]];
		int x = 1;

		while (cnt >= x)
		{
			res++;
			cnt -= x;
			x++;
		}
	}

	cout << res << endl;
}