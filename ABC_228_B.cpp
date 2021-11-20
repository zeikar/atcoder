#include <iostream>
using namespace std;

int a[100001];
int secret[100001];

int main() {
	int n, x;
	cin >> n >> x;

	for (int i = 1; i <= n; i++)
	{
		cin >> a[i];
	}

	int ans = 1;
	secret[x] = 1;

	int next = a[x];
	while (secret[next] == 0)
	{
		ans++;
		secret[next] = 1;
		next = a[next];
	}

	cout << ans << endl;
}