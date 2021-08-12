#include <iostream>
#include <string>
#include <climits>
using namespace std;

int main() {
	string m;
	long long x;

	cin >> m >> x;

	int mdigit = 0;
	for (int i = 0; i < m.length(); i++)
	{
		mdigit = max(mdigit, m[i] - '0');
	}

	if (m.size() == 1)
	{
		cout << (m[0] - '0' <= x) << endl;
		return 0;
	}

	long long start = mdigit + 1;
	long long end = 1e18;

	while (start <= end)
	{
		long long mid = (start + end) / 2;

		// m to number
		long long num = 0;
		bool overflowed = false;
		for (int i = 0; i < m.length(); i++)
		{
			// check overflow
			if (num >= LLONG_MAX / mid)
			{
				overflowed = true;
				break;
			}

			num *= mid;
			num += m[i] - '0';
		}

		if (num <= x && !overflowed)
		{
			start = mid + 1;
		}
		else
		{
			end = mid - 1;
		}
	}

	cout << end - mdigit << endl;
}