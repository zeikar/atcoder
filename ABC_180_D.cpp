#include <iostream>
#include <algorithm>
#include <climits>
#include <queue>
using namespace std;

int main()
{
	unsigned long long x, y, a, b;
	unsigned long long ans = 0;

	cin >> x >> y >> a >> b;

	queue< pair<unsigned long long, unsigned  long long> >que;

	que.push(make_pair(x, 0));

	while (!que.empty())
	{
		unsigned long long str = que.front().first;
		unsigned long long exp = que.front().second;
		que.pop();

		//cout << str << ' ' << exp << endl;

		if (str >= y)
		{
			continue;
		}

		ans = max(ans, exp);

		// overflow
		if (!(ULLONG_MAX / a < str))
		{
			// multiple
			que.push(make_pair(str * a, exp + 1));
		}

		// add....
		unsigned long long c = (y - str) / b;
		if (c == 0)
		{
			que.push(make_pair(str + b, exp + 1));
		}
		else if ((y - str) % b == 0 && c > 1) {
			c--;
			que.push(make_pair(str + b * c, exp + c));
		}
		else
		{
			que.push(make_pair(str + b * c, exp + c));
		}
	}

	cout << ans << endl;
}