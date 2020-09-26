#include <iostream>
#include <map>
#include <climits>
#include <algorithm>
#include <cmath>
using namespace std;

int arr[300000];
int maxlen[300000];
const int N = 300000;  // limit for array size
int t[4 * N + 1];

void buildTree(int* tree, int index, int s, int e)
{
	//base case
	if (s > e)
		return;
	//reached leaf node
	if (s == e)
	{
		tree[index] = 0;
		return;
	}
	//now build the segment tree in bottom up manner
	int m = (s + e) / 2;
	buildTree(tree, 2 * index, s, m);
	buildTree(tree, 2 * index + 1, m + 1, e);
	tree[index] = max(tree[2 * index], tree[2 * index + 1]);
	return;
}

//function to query the segment tree for RMQ
int query(int* tree, int index, int s, int e, int qs, int qe)
{
	//base case: if query range is outside the node range
	if (qs > e || s > qe)
		return INT_MIN;
	//complete overlap
	if (s >= qs && e <= qe)
		return tree[index];
	//now partial overlap case is executed
	int m = (s + e) / 2;
	int left_ans = query(tree, 2 * index, s, m, qs, qe);
	int right_ans = query(tree, 2 * index + 1, m + 1, e, qs, qe);
	return max(left_ans, right_ans);
}

//function to update a value at position to "pos"
void updateNode(int* tree, int index, int s, int e, int pos, int val)
{
	if (pos<s || pos>e)
		return;
	if (s == e)
	{
		tree[index] = val;
		return;
	}
	int m = (s + e) / 2;
	updateNode(tree, 2 * index, s, m, pos, val);
	updateNode(tree, 2 * index + 1, m + 1, e, pos, val);
	tree[index] = max(tree[2 * index], tree[2 * index + 1]);
	return;
}

int main() {
	int result = 0;
	int n, k;
	cin >> n >> k;

	for (int i = 0; i < n; i++)
	{
		cin >> arr[i];
	}

	int s = 0, e = 300000;
	buildTree(t, 1, 0, 300000);

	for (int i = 0; i < n; i++)
	{
		int a = arr[i];

		int maxim = max(query(t, 1, s, e, a-k, a+k), 0);
		updateNode(t, 1, s, e, a, maxim + 1);

		maxlen[i] = maxim + 1;
		result = max(result, maxlen[i]);
	}

	//debug
	/*for (int i = 0; i < n; i++)
	{
		cout << maxlen[i] << ' ';
	}
	cout << endl;*/

	cout << result << endl;
}