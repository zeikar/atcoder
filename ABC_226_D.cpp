#include <iostream>
#include <map>
#include <utility>
#include <cmath>
using namespace std;

int coordsx[500], coordsy[500];
map<pair<int, int>, bool> spells;

int gcd(int a, int b) {
	if (b == 0) {
		return a;
	}
	return gcd(b, a % b);
}

void learnSpell(int dx, int dy) {
	int g = gcd(abs(dx), abs(dy));

	dx /= g;
	dy /= g;

	spells[make_pair(dx, dy)] = true;
}

int main() {
	int n;
	cin >> n;
	for (int i = 0; i < n; i++)
	{
		cin >> coordsx[i] >> coordsy[i];
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (i == j)
			{
				continue;
			}

			int dx = coordsx[j] - coordsx[i];
			int dy = coordsy[j] - coordsy[i];

			learnSpell(dx, dy);
		}
	}

	cout << spells.size() << endl;
}