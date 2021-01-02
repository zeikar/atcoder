#include <iostream>
#include <string>
using namespace std;

int main() {
	int n, x;
	string str;

	cin >> n >> x >> str;

	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == 'o')
		{
			x++;
		}
		else if (x > 0)
		{
			x--;
		}
	}

	cout << x << endl;
}