#include <iostream>
#include <cmath>
using namespace std;

int main() {
	int r1, c1, r2, c2;
	cin >> r1 >> c1 >> r2 >> c2;
	int r = r2 - r1, c = c2 - c1;

	if (r == 0 && c == 0)
	{
		cout << 0 << endl;
	}
	else if (abs(r) == abs(c))
	{
		cout << 1 << endl;
	}
	else if (abs(r) + abs(c) <= 3)
	{
		cout << 1 << endl;
	}
	else if ((r1 + c1 + r2 + c2) % 2 == 0)
	{
		cout << 2 << endl;
	}
	else if (abs(r) + abs(c) <= 6)
	{
		cout << 2 << endl;
	}
	else if(abs(r+c) <= 3 || abs(r-c) <= 3)
	{
		cout << 2 << endl;
	}
	else
	{
		cout << 3 << endl;
	}
}