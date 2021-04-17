#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <algorithm>
#include <map>
#include <vector>
#include <utility>
#include <set>

using namespace std;

long long c[200001];

vector< pair<long long, long long> > day;
long long haveToPay = 0;

int main() {
	long long n, snuke;

	scanf("%lld %lld", &n, &snuke);

	long long result = 0;

	for (int i = 1; i < n + 1; i++)
	{
		long long a, b;

		scanf("%lld %lld %lld", &a, &b, &c[i]);


		day.push_back(make_pair(a, i));
		day.push_back(make_pair(b + 1, -i));
	}

	sort(day.begin(), day.end());

	int lastTime = day.begin()->first - 1;
	for (int i = 0; i < day.size(); i++)
	{
		int currentTime = day[i].first;

		// PAY
		int dur = currentTime - lastTime;

		// snuke
		if (haveToPay > snuke)
		{
			result += snuke * dur;
		}
		else
		{
			result += haveToPay * dur;
		}

		int v = day[i].second;

		if (v > 0)
		{
			haveToPay += c[v];
		}
		else
		{
			haveToPay -= c[-v];
		}

		lastTime = currentTime;
	}

	printf("%lld\n", result);
}