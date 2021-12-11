#include <iostream>
#include <vector>
#include <map>
using namespace std;

struct Node
{
	int parent;
	int rank;
};
Node DSU[100001];

//Struktura za rad sa disjunktnim skupovima
//Slozenost: Amortizovano O(alfa(N)) po operaciji

inline void MakeSet(int x)
{
	DSU[x].parent = x;
	DSU[x].rank = 0;
}

inline int Find(int x)
{
	if (DSU[x].parent == x) return x;
	DSU[x].parent = Find(DSU[x].parent);
	return DSU[x].parent;
}

inline void Union(int x, int y)
{
	int xRoot = Find(x);
	int yRoot = Find(y);
	if (xRoot == yRoot) return;

	if (DSU[xRoot].rank < DSU[yRoot].rank)
	{
		DSU[xRoot].parent = yRoot;
	}
	else if (DSU[xRoot].rank > DSU[yRoot].rank)
	{
		DSU[yRoot].parent = xRoot;
	}
	else
	{
		DSU[yRoot].parent = xRoot;
		DSU[xRoot].rank++;
	}
}

int cnt[100001];

int main() {
	int n, m;
	cin >> n >> m;

	for (int i = 1; i <= n; i++)
	{
		MakeSet(i);
	}

	string ans = "Yes";
	while (m--)
	{
		int a, b;
		cin >> a >> b;

		if (Find(a) == Find(b))
		{
			ans = "No";
		}

		Union(a, b);

		if (cnt[a] >= 2 || cnt[b] >= 2)
		{
			ans = "No";
		}

		cnt[a]++;
		cnt[b]++;
	}

	cout << ans << endl;
}