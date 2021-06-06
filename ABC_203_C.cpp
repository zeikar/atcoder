#include <iostream>
#include <map>
using namespace std;

map<long long, long long> f;

int main() {
	long long n, k;

	cin >> n >> k;

	for (int i = 0; i < n; i++)
	{
		long long a, b;
		cin >> a >> b;
		f[a] += b;
	}

	map<long long, long long>::iterator it = f.begin();
	long long cur = 0;

	while (it != f.end())
	{
		long long a = it->first;
		long long b = it->second;

		if (cur + k < a)
		{
			break;
		}

		// k 이전은 다 더함
		while (it != f.end() && it->first <= cur + k)
		{
			a = it->first;
			b = it->second;

			long long cost = a - cur;
			cur = a;
			k -= cost;

			k += b;
			it++;
		}

	}
	cur += k;

	cout << cur << endl;
}