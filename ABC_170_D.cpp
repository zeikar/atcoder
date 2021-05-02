#include <iostream>
using namespace std;

bool check[1000001];
bool duplicate[1000001];

int main() {
	int n;
	cin >> n;
	for (int i = 0; i < n; i++)
	{
		int x;
		cin >> x;

		if (check[x])
		{
			duplicate[x] = true;
		}

		check[x] = true;
	}

	for (int i = 1; i <= 1000000; i++)
	{
		int num = i;
		if (!check[i])
		{
			continue;
		}

		while (num + i <= 1000000)
		{
			num += i;
			check[num] = false;
		}
	}

	int res = 0;
	for (int i = 1; i <= 1000000; i++)
	{
		res += check[i] && !duplicate[i];
	}
	cout << res << endl;
}