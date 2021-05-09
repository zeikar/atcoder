#include <iostream>
using namespace std;

long long cnt[200];

int main() {
	int n;
	cin >> n;

	for (int i = 0; i < n; i++)
	{
		int a;
		cin >> a;

		cnt[a % 200]++;
	}

	long long result = 0;
	for (int i = 0; i < 200; i++)
	{
		result += (cnt[i] * (cnt[i] - 1)) / 2;
	}
	cout << result << endl;
}