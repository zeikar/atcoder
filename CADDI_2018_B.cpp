#include <iostream>
#include <string>
using namespace std;

int a[100000];

int main() {
	int n;
	cin >> n;
	for (int i = 0; i < n; i++)
	{
		cin >> a[i];
	}

	string ans = "second";
	for (int i = 0; i < n; i++)
	{
		if (a[i] % 2 != 0)
		{
			ans = "first";
			break;
		}
	}

	cout << ans << endl;
}