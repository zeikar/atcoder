#include <iostream>
#include <cmath>
#include <string>
using namespace std;

int a[100000], b[100000];

int main() {
	int n;

	cin >> n;

	for (int i = 0; i < n; i++)
	{
		cin >> a[i];
	}
	for (int i = 0; i < n; i++)
	{
		cin >> b[i];
	}

	long long res = 0;

	for (int i = 0; i < n; i++)
	{
		res += a[i] * b[i];
	}

	if (res == 0)
	{
		cout << "Yes" << endl;
	}
	else
	{
		cout << "No" << endl;
	}
}