#include <iostream>
#include <algorithm>
using namespace std;

long long arr[200000];

int main() {
	int n;
	cin >> n;

	for (int i = 0; i < n; i++)
	{
		cin >> arr[i];
	}

	sort(arr, arr + n);

	long long result = 0;

	for (int i = 0; i < n; i++)
	{
		result -= arr[i] * (n - i - 1);
		result += arr[i] * (i);
	}

	cout << result << endl;
}