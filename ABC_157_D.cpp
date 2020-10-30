#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

vector<int> friends[100001];
vector<int> blocks[100001];
int friendChunk[100001];
int chunkCnt[100001];
int blockCnt[100001];

int main() {
	int n, m, k;
	cin >> n >> m >> k;

	for (int i = 0; i < m; i++)
	{
		int a, b;
		cin >> a >> b;
		friends[a].push_back(b);
		friends[b].push_back(a);
	}

	for (int i = 0; i < k; i++)
	{
		int a, b;
		cin >> a >> b;
		blocks[a].push_back(b);
		blocks[b].push_back(a);
	}

	// bfs �� ģ�� ����� ���Ѵ�.
	int chunk = 1;
	for (int i = 1; i <= n; i++)
	{
		if (friendChunk[i] != 0)
		{
			continue;
		}

		queue<int> que;
		que.push(i);
		friendChunk[i] = chunk;
		chunkCnt[chunk]++;

		while (!que.empty())
		{
			int node = que.front();
			que.pop();

			for (int j = 0; j < friends[node].size(); j++)
			{
				int next = friends[node][j];

				if (friendChunk[next] != 0)
				{
					continue;
				}

				friendChunk[next] = chunk;
				chunkCnt[chunk]++;
				que.push(next);
			}
		}

		++chunk;
	}

	// ���� ģ�� ��� �� block ������ ���Ѵ�.
	for (int i = 1; i <= n; i++)
	{
		for (int j = 0; j < blocks[i].size(); j++)
		{
			if (friendChunk[i] == friendChunk[blocks[i][j]])
			{
				++blockCnt[i];
			}
		}
	}

	// ��ü ��� ���� - �̹� ģ���� ���� - ��ϵ� ���� - 1
	for (int i = 1; i <= n; i++)
	{
		cout << chunkCnt[friendChunk[i]] - friends[i].size() - blockCnt[i] - 1 << ' ';
	}
	cout << endl;
}