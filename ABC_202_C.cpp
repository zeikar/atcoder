#include <iostream>
using namespace std;

long long a[100000], b[100000], c[100000];
long long acnt[100001], bcnt[100000];

int main() {
	int n;
	cin >> n;
	for (int i = 0; i < n; i++)
	{
		cin >> a[i];
		a[i]--;
		acnt[a[i]]++;
	}
	for (int i = 0; i < n; i++)
	{
		cin >> b[i];
		b[i]--;
	}
	for (int i = 0; i < n; i++)
	{
		cin >> c[i];
		c[i]--;
		bcnt[b[c[i]]]++;
	}

	long long result = 0;
	for (int i = 0; i < n; i++)
	{
		result += (acnt[i] * bcnt[i]);
	}

	cout << result << endl;
}