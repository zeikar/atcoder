#include <iostream>
#include <algorithm>
#include <utility>
using namespace std;

pair<long long, pair<long long, long long>> votes[200000];

int main() {
	int n;
	cin >> n;

	for (int i = 0; i < n; i++)
	{
		cin >> votes[i].second.first >> votes[i].second.second;
		votes[i].first = votes[i].second.second + votes[i].second.first + votes[i].second.first;
	}

	sort(votes, votes + n);

	long long aoki = 0, takahashi = 0;

	for (int i = 0; i < n; i++)
	{
		aoki += votes[i].second.first;
	}

	int idx = n - 1;
	int cnt = 0;

	while (takahashi <= aoki)
	{
		aoki -= votes[idx].second.first;
		takahashi += votes[idx].second.first + votes[idx].second.second;

		idx -= 1;
		cnt++;
	}

	cout << cnt << endl;
}