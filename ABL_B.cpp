#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

int main()
{
	long long a, b, c, d;
	cin >> a >> b >> c >> d;

	if ((a <= c && c <= b) || (c <= a && a <= d))
	{
		cout << "Yes" << endl;
	}
	else
	{
		cout << "No" << endl;
	}
}