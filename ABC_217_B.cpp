#include <iostream>
#include <string>
using namespace std;

int main() {
	string a[3];
	cin >> a[0] >> a[1] >> a[2];

	string as[4] = {"ABC", "ARC", "AGC", "AHC"};

	for (int i = 0; i < 4; i++)
	{
		bool found = false;
		for (int j = 0; j < 3; j++)
		{
			if (as[i] == a[j])
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			cout << as[i] << endl;
		}
	}
}