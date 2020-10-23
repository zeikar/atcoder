#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
	long long n;
	vector<long long > d;

	cin >> n;

	for (long long i = 1; i * i <= n; i++)
	{
		if (n % i == 0)
		{
			if (i == n / i)
			{
				d.push_back(i);

			}
			else
			{
				d.push_back(i);
				d.push_back(n / i);

			}
		}
	}

	sort(d.begin(), d.end());

	for (int i = 0; i < d.size(); i++)
	{
		cout << d[i] << endl;
	}
}