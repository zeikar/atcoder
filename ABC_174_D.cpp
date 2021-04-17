#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
	int n;
	string str;
	cin >> n >> str;

	vector<int> w;
	vector<int> r;

	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] == 'W')
		{
			w.push_back(i);
		}
		else
		{
			r.push_back(i);
		}
	}

	int count = 0;
	int widx = 0;
	int ridx = r.size() - 1;

	while (widx < w.size() && ridx >= 0 && w[widx] < r[ridx])
	{
		++widx;
		--ridx;

		++count;
	}

	cout << count << endl;
}