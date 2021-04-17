#include <iostream>
using namespace std;

int n, k;
int p[5000];
int c[5000];

int cycle[5000];
long long cycleSum[5000];
int cycleCount[5000];
bool visited[5000];

int main() {
	cin >> n >> k;
	for (int i = 0; i < n; i++)
	{
		cin >> p[i];
		p[i]--;
	}

	for (int i = 0; i < n; i++)
	{
		cin >> c[i];
	}

	// search cycle
	int cycleIdx = 0;
	for (int i = 0; i < n; i++)
	{
		int current = i;
		long long sum = c[current];
		int cycleCnt = 1;

		if (visited[current])
		{
			continue;
		}

		visited[current] = true;
		cycle[current] = cycleIdx;

		while (true)
		{
			int next = p[current];

			if (visited[next])
			{
				break;
			}

			visited[next] = true;
			cycle[next] = cycleIdx;
			sum += c[next];
			current = next;

			cycleCnt++;
		}

		cycleCount[cycleIdx] = cycleCnt;
		cycleSum[cycleIdx] = sum;

		cycleIdx++;
	}

	// solve
	long long result = -98765432123456789LL;
	for (int i = 0; i < n; i++)
	{
		long long sum = 0;
		int current = i;
		int cnt = k;

		double multiple = (double)cnt / cycleCount[cycle[i]];
		if (multiple > 2) {
			if (cycleSum[cycle[i]] > 0) {
				sum += ((int)multiple - 1) * cycleSum[cycle[i]];
			}
			cnt -= ((int)multiple - 1) * cycleCount[cycle[i]];
		}

		for (int j = 0; j < cnt; j++)
		{
			int next = p[current];
			sum += c[next];
			current = next;

			result = max(result, sum);
		}

		result = max(result, sum);
	}

	cout << result << endl;
}