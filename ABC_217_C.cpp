#include <iostream>
#include <string>
using namespace std;

int a[200001];
int b[200001];

int main() {
	int n;
	cin >> n;
	for (int i = 1; i <= n; i++)
	{
		cin >> a[i];
	}

	for (int i = 1; i <= n; i++)
	{
		b[a[i]] = i;
	}

	for (int i = 1; i <= n; i++)
	{
		cout << b[i] << ' ';
	}
	cout << endl;
}