#include <iostream>
#include <string>
using namespace std;

int main() {
	int n;
	string s;
	long long result;

	cin >> n >> s;

	// step 1, count rgb triplets
	long long cntR = 0, cntG = 0, cntB = 0;

	for (int i = 0; i < n; i++)
	{
		if (s[i] == 'R')
		{
			cntR++;
		}
		else if (s[i] == 'G')
		{
			cntG++;
		}
		else
		{
			cntB++;
		}
	}

	result = cntR * cntG * cntB;

	// step 2, count j-i == k-j
	for (int i = 0; i < n; i++)
	{
		for (int offset = 1; offset < n; offset++)
		{
			int j = i + offset;
			int k = j + offset;

			if (k >= n)
			{
				continue;
			}

			if (s[i] != s[j] && s[j] != s[k] && s[i] != s[k])
			{
				result--;
			}
		}
	}

	cout << result << endl;
}