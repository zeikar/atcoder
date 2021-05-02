#include <iostream>
#include <algorithm>
using namespace std;

int a[100];
int b[100];

int main() {
	int n;
	cin >> n;

	int minim = 987654321;
	int maxim = 0;
	for (int i = 0; i < n; i++)
	{
		cin >> a[i];
	}
	for (int i = 0; i < n; i++)
	{
		cin >> b[i];
	}

	for (int i = 0; i < n; i++)
	{
		minim = min(minim, b[i]);
		maxim = max(maxim, a[i]);
	}

	if (minim < maxim)
	{
		cout << 0 << endl;
	}
	else
	{
		cout << minim - maxim + 1 << endl;
	}
}