#include <iostream>
#include <vector>
using namespace std;

int T[200000];
vector<int> toLearn[200000];
bool learned[200000];

long long learn(int idx) {
	if (learned[idx])
	{
		return 0;
	}

	long long ret = 0;

	for (int i = 0; i < toLearn[idx].size(); i++)
	{
		ret += learn(toLearn[idx][i]);
	}

	learned[idx] = true;
	return ret + T[idx];
}

int main() {
	int n;
	cin >> n;

	for (int i = 0; i < n; i++)
	{
		cin >> T[i];

		int k;
		cin >> k;
		while (k--)
		{
			int a;
			cin >> a;
			toLearn[i].push_back(a - 1);
		}
	}

	cout << learn(n - 1) << endl;
}