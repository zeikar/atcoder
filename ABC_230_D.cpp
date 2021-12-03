#include <iostream>
#include <algorithm>
#include <utility>
#include <queue>
#include <vector>
using namespace std;

int main() {
	int n, d;
	cin >> n >> d;

	vector< pair<int, int> > walls;

	for (int i = 1; i <= n; i++)
	{
		int l, r;
		cin >> l >> r;

		walls.push_back(make_pair(r, l));
	}

	sort(walls.begin(), walls.end());

	int ans = 0, x = -1987654321;
	for (int i = 0; i < walls.size(); i++)
	{
		int l = walls[i].second;
		int r = walls[i].first;

		if (l > x + d - 1)
		{
			ans++;

			x = r;
		}
	}

	cout << ans << endl;
}