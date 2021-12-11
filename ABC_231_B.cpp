#include <iostream>
#include <string>
#include <map>
using namespace std;

int main() {
	int n;
	cin >> n;

	map<string, int> vote;
	for (int i = 0; i < n; i++)
	{
		string s;
		cin >> s;
		vote[s] ++;
	}

	string ans = "";
	int maxVote = 0;
	for (auto const& x : vote)
	{
		if (maxVote < x.second)
		{
			ans = x.first;
			maxVote = x.second;
		}
	}

	cout << ans << endl;
}