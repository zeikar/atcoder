#include <iostream>
#include <climits>
using namespace std;

int main() {
	int n;
	cin >> n;

	int maxPlus = -INT_MAX;
	int minPlus = INT_MAX;
	int maxMinus = -INT_MAX;
	int minMinus = INT_MAX;
	for (int i = 0; i < n; i++)
	{
		int x, y;
		cin >> x >> y;

		int plus = x + y;
		int minus = x - y;

		maxPlus = max(maxPlus, plus);
		minPlus = min(minPlus, plus);
		maxMinus = max(maxMinus, minus);
		minMinus = min(minMinus, minus);
	}

	cout << max(maxPlus - minPlus, maxMinus - minMinus) << endl;
}