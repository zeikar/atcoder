#include <iostream>
using namespace std;

int main() {
	int n, p;
	cin >> n >> p;

	int cnt = 0;

	for (int i = 0; i < n; i++)
	{
		int a;
		cin >> a;
		if (a < p)
		{
			++cnt;
		}
	}

	cout << cnt << endl;
}