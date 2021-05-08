#include <iostream>
#include <vector>
#include <string>
using namespace std;

vector<int> people[200002];
int p[200001];

int main() {
	int n, w;
	cin >> n >> w;

	for (int i = 1; i <= n; i++)
	{
		int s, t;
		cin >> s >> t >> p[i];

		people[s].push_back(i);
		people[t].push_back(-i);
	}

	string result = "Yes";
	long long current = 0;
	for (int i = 0; i <= 200001; i++)
	{
		if (current > w)
		{
			result = "No";
			break;
		}

		for (int j = 0; j < people[i].size(); j++)
		{
			int a = people[i][j];

			if (a < 0)
			{
				current -= p[-a];
			}
			else
			{
				current += p[a];
			}
		}
	}

	cout << result << endl;
}