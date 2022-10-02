#include <iostream>
#include <map>
using namespace std;

int a[200000];
map<int, int> diff;

int main() {
	int n;
	cin >> n;

	long long res = 0;
	for (int i = 0; i < n; i++)
	{
		cin >> a[i];

		res += diff[i - a[i]];
		diff[a[i] + i]++;
	}


	cout << res << endl;
}