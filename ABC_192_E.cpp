#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <climits>
using namespace std;

class Edge
{
public:
	Edge(int dest, int t, int k) {
		this->dest = dest;
		this->t = t;
		this->k = k;
	}

	int dest;
	int t;
	int k;
};

vector<Edge> edges[100001];
long long distances[100001];

int main() {
	int n, m, x, y;
	cin >> n >> m >> x >> y;

	for (int i = 0; i <= n; i++)
	{
		distances[i] = LLONG_MAX;
	}

	while (m--)
	{
		int a, b, t, k;
		cin >> a >> b >> t >> k;

		edges[a].push_back(Edge(b, t, k));
		edges[b].push_back(Edge(a, t, k));
	}

	priority_queue<tuple<long long, int> > pq;
	pq.push(make_tuple(0, x));
	distances[x] = 0;

	while (!pq.empty())
	{
		auto current = pq.top();
		pq.pop();

		long long currentTime = -get<0>(current);
		int currentCity = get<1>(current);

		if (distances[currentCity] < currentTime)
		{
			continue;
		}

		for (int i = 0; i < edges[currentCity].size(); i++)
		{
			auto nextEdge = edges[currentCity][i];
			long long nextTime;

			if (currentTime % nextEdge.k == 0)
			{
				nextTime = currentTime + nextEdge.t;
			}
			else
			{
				nextTime = ((currentTime / nextEdge.k) + 1) * nextEdge.k + nextEdge.t;
			}

			if (distances[nextEdge.dest] <= nextTime)
			{
				continue;
			}
			distances[nextEdge.dest] = nextTime;

			pq.push(make_tuple(-nextTime, nextEdge.dest));
		}
	}

	if (distances[y] == LLONG_MAX)
	{
		cout << -1 << endl;
	}
	else
	{
		cout << distances[y] << endl;
	}
}