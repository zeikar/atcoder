#include <iostream>
#include <algorithm>
using namespace std;

int x[100];
int y[100];

int main() {
	int n;

	cin >> n;

	for (int i = 0; i < n; i++)
	{
		int a, b;

		cin >> a >> b;

		x[i] = a;
		y[i] = b;
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			for (int k = 0; k < n; k++)
			{
				if (i == j || j == k || i == k)
				{
					continue;
				}

				if ((y[j] - y[i]) == 0 && (y[k] - y[j]) == 0)
				{
					cout << "Yes" << endl;
					return 0;
				}
				else if ((y[j] - y[i]) == 0 || (y[k] - y[j]) == 0)
				{
					continue;
				}


				if ((double)((double)x[j] - x[i]) / ((double)y[j] - y[i]) == (double)((double)x[k] - x[j]) / ((double)y[k] - y[j]))
				{
					cout << "Yes" << endl;
					return 0;
				}
			}
		}
	}

	cout << "No" << endl;
}