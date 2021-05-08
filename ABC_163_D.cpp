#include <iostream>
using namespace std;

int prefix[200002];

int main() {
	int n, k;
	cin >> n >> k;

	n++;
	for (int i = 1; i <= n; i++)
	{
		prefix[i] = (prefix[i - 1] + i) % (int)(1e9 + 7);
	}

	long long result = 0;
	for (int i = k; i <= n; i++)
	{
		result += (prefix[n] + (int)(1e9 + 7) - prefix[n - i]) % (int)(1e9 + 7) + (int)(1e9 + 7) - (prefix[i] + (int)(1e9 + 7) - prefix[0]) % (int)(1e9 + 7) + 1;
		result %= (int)(1e9 + 7);
	}

	cout << result << endl;
}