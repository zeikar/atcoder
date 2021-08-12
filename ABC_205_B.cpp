#include <iostream>
#include <string>
using namespace std;

bool visited[1001];

int main() {
	int n;
	cin >> n;

	string res = "Yes";

	for (int i = 0; i < n; i++)
	{
		int a;
		cin >> a;

		if (a < 1 || a > n)
		{
			res = "No";
			break;;
		}

		if (visited[a])
		{
			res = "No";
			break;
		}

		visited[a] = true;
	}

	cout << res << endl;
}