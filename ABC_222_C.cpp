#include <iostream>
#include <string>
#include <utility>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;

int wincnt[100];
string gcp[100];


bool isP1Win(int round, int p1, int p2) {
	return (gcp[p1][round] == 'G' && gcp[p2][round] == 'C')
		|| (gcp[p1][round] == 'C' && gcp[p2][round] == 'P')
		|| (gcp[p1][round] == 'P' && gcp[p2][round] == 'G');
}

int main() {
	int n, m;
	cin >> n >> m;

	for (int i = 0; i < 2 * n; i++)
	{
		cin >> gcp[i];
	}

	priority_queue< pair<int, int> > currentTurn, nextTurn;
	for (int i = 0; i < 2 * n; i++)
	{
		currentTurn.push(make_pair(0, i));
	}

	for (int i = 0; i < m; i++)
	{
		while (!currentTurn.empty())
		{
			int p1 = currentTurn.top().second;
			currentTurn.pop();
			int p2 = currentTurn.top().second;
			currentTurn.pop();

			bool p1win = isP1Win(i, p1, p2);
			bool p2win = isP1Win(i, p2, p1);

			if (p1win) {
				wincnt[p1]++;
			}
			if (p2win) {
				wincnt[p2]++;
			}

			nextTurn.push(make_pair(-wincnt[p1], p1));
			nextTurn.push(make_pair(-wincnt[p2], p2));
		}

		currentTurn = nextTurn;
		nextTurn = priority_queue< pair<int, int> >();
	}

	vector< pair<int, int> > tosort;
	for (int i = 0; i < 2 * n; i++)
	{
		tosort.push_back(make_pair(-wincnt[i], i));
	}

	sort(tosort.begin(), tosort.end());

	for (int i = 0; i < 2 * n; i++)
	{
		cout << tosort[i].second + 1 << endl;
	}
}