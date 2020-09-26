#include <iostream>
#include <algorithm>
using namespace std;

int arr[200000] = { 0 };

int main()
{
	int n;
	long long result = 0;

	cin >> n;
	for (int i = 0; i < n; i++)
	{
		cin >> arr[i];
	}

	sort(arr, arr + n);

	int maxim = n - 1;

	for (int i = 1; i < n; i++)
	{
		if (i % 2 == 0)
		{
			maxim--;
		}

		result += arr[maxim];
	}

	cout << result << endl;
}