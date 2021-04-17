#include <iostream>
#include <cmath>
#include <string>
using namespace std;

int n;
int arr[1 << 17];

int win(int a, int b) {
	if (b - a <= 1)
	{
		if (arr[a] > arr[b])
		{
			return a;
		}
		else
		{
			return b;
		}
	}

	int mid = ((a + b) / 2);
	int l = win(a, mid);
	int r = win(mid + 1, b);

	if (arr[l] > arr[r])
	{
		return l;
	}
	else
	{
		return r;
	}
}

int solve() {
	int total = (1 << n) - 1;

	int a = win(0, total / 2);
	int b = win(total / 2 + 1, total);

	if (arr[a] > arr[b])
	{
		return b + 1;
	}
	else
	{
		return a + 1;
	}
}

int main() {
	cin >> n;

	for (int i = 0; i < (1 << n); i++)
	{
		cin >> arr[i];
	}

	cout << solve() << endl;
}