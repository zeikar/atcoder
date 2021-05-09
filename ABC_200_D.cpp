#include <iostream>
#include <vector>
using namespace std;

int n;
int a[200];
vector<int> remains[200];

int main() {
	cin >> n;

	for (int i = 0; i < n; i++)
	{
		cin >> a[i];
		a[i] %= 200;
	}

	vector<int> b[200];

	for (int i = 1; i <= (1 << min(8, n)); i++)
	{
		vector<int> temp;
		long long tempSum = 0;

		for (int j = 0; j < min(n, 8); j++)
		{
			if (i & (1 << j))
			{
				temp.push_back(j + 1);
				tempSum = (tempSum + a[j]) % 200;
			}
		}

		if (b[tempSum].size() == 0)
		{
			b[tempSum] = temp;
		}
		else
		{
			cout << "Yes" << endl;
			cout << b[tempSum].size() << ' ';
			for (int i = 0; i < b[tempSum].size(); i++)
			{
				cout << b[tempSum][i] << ' ';
			}
			cout << endl;
			cout << temp.size() << ' ';
			for (int i = 0; i < temp.size(); i++)
			{
				cout << temp[i] << ' ';
			}
			cout << endl;
			return 0;
		}
	}

	cout << "No" << endl;
}