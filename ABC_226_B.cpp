#include <iostream>
#include <vector>
#include <map>
using namespace std;

int main() {
	int n;
	cin >> n;

	map<vector<int>, bool> visited;

	for (int i = 0; i < n; i++)
	{
		vector<int> arr;
		int l;
		cin >> l;
		while (l--)
		{
			int a;
			cin >> a;
			arr.push_back(a);
		}

		visited[arr] = true;
	}

	cout << visited.size() << endl;
}