#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
using namespace std;

int a[200001];
pair<int, int> map[200001];
bool visited[200001];

int main() {
	int n;
	cin >> n;


	for (int i = 0; i < n; i++)
	{
		cin >> a[i];
		map[i].first = a[i];
		map[i].second = i;
	}

	sort(map, map + n);

	vector<int> ans;

	for (int i = n - 1; i > 0; i--)
	{
		int val = map[i].first;
		int pos = map[i].second;

		// move
		if (pos < val - 1 && !visited[pos])
		{
			// swap
			int target = a[pos + 1];

			map[i].second = map[i - (val - target)].second;
			map[i - (val - target)].second = pos;

			a[pos] = target;
			a[pos + 1] = val;

			ans.push_back(pos);
			visited[pos] = true;
			i++;
		}
		else if (pos == val - 1)
		{
			continue;
		}
		else
		{
			ans.clear();
			break;
		}
	}

	if (ans.size() != n - 1)
	{
		cout << -1 << endl;
	}
	else
	{
		for (int i = 0; i < ans.size(); i++)
		{
			cout << ans[i] + 1 << endl;
		}
	}
}