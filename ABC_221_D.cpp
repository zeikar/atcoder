#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
using namespace std;

int a[200000];
int b[200000];
int results[200001];
bool loggedin[200001];

int main() {
	int n;

	cin >> n;

	// 1 : µé¾î¿È -1: ³ª°¨
	map<int, vector<int> > events;

	for (int i = 0; i < n; i++)
	{
		cin >> a[i] >> b[i];

		events[a[i]].push_back(i + 1);
		events[a[i] + b[i]].push_back(- (i + 1));
	}

	int cnt = 0;
	int prevTime = -1;

	for (auto const& x : events)
	{
		if (prevTime == -1)
		{
			prevTime = x.first - 1;
		}

		results[cnt] += x.first - 1 - prevTime;

		for (int i = 0; i < x.second.size(); i++)
		{
			if (x.second[i] > 0)
			{
				cnt += 1;
			}
			if (x.second[i] < 0)
			{
				cnt -= 1;
			}
		}

		results[cnt] += 1;
		prevTime = x.first;

	}

	for (int i = 1; i <= n; i++)
	{
		cout << results[i] << ' ';
	}
	cout << endl;
}