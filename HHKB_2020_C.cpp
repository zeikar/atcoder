#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>
using namespace std;

int p[200000];
bool visited[200001];

int main() {
	int n;
	int minim = 0;

	cin >> n;

	for (int i = 0; i < n; i++)
	{
		cin >> p[i];
	}

	for (int i = 0; i < n; i++)
	{
		int num = p[i];

		visited[num] = true;

		while (visited[minim])
		{
			++minim;
		}

		cout << minim << endl;
	}
}