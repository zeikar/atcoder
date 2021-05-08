#include <iostream>
using namespace std;

long long arr[200000];
long long p[200001];
long long mp[200001];

int main() {
	int n;
	cin >> n;

	for (int i = 0; i < n; i++)
	{
		cin >> arr[i];
	}

	for (int i = 1; i <= n; i++)
	{
		p[i] = p[i - 1] + arr[i - 1];
		mp[i] = max(mp[i - 1], p[i]);
	}

	long long result = 0;
	long long sum = 0;
	for (int i = 1; i <= n; i++)
	{
		result += p[i];

		if (i != n)
		{
			sum = max(sum, max(result, result + mp[i]));
		}
		else
		{
			sum = max(sum, result);
		}
	}

	cout << sum << endl;
}