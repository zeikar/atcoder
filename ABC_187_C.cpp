#include <iostream>
#include <string>
#include <map>
using namespace std;

string str[200000];

int main() {
	int n;

	cin >> n;

	map<string, bool> visited;

	string result = "";

	for (int i = 0; i < n; i++)
	{
		cin >> str[i];
		visited[str[i]] = true;
	}

	for (int i = 0; i < n; i++)
	{
		string compare = str[i];

		if (compare[0] == '!')
		{
			compare = compare.substr(1);
		}

		if (visited[compare] && visited["!" + compare])
		{
			result = compare;
			break;
		}
	}

	if (result == "")
	{
		cout << "satisfiable" << endl;
	}
	else
	{
		cout << result << endl;
	}
}