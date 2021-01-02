#include <iostream>
#include <algorithm>
using namespace std;

int x[1000], y[1000];

double slope(int a1, int b1, int a2, int b2) {
	return (double)(b2 - b1) / (a2 - a1);
}

int main() {
	int n;
	cin >> n;

	for (int i = 0; i < n; i++)
	{
		cin >> x[i] >> y[i];
	}

	int result = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			if (i == j)
			{
				continue;
			}

			if (slope(x[i], y[i], x[j], y[j]) >= -1 && slope(x[i], y[i], x[j], y[j]) <= 1)
			{
				++result;
			}
		}
	}

	cout << result << endl;
}