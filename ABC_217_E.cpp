#include <iostream>
#include <queue>
#include <vector>
#include <functional>
using namespace std;

int main() {
	int q;
	cin >> q;

	priority_queue <int, vector<int>, greater<int> > pq;
	queue<int> buffer;

	while (q--)
	{
		int a;
		cin >> a;

		if (a == 1)
		{
			int x;
			cin >> x;
			buffer.push(x);
		}
		else if (a == 2)
		{
			if (pq.empty())
			{
				cout << buffer.front() << endl;
				buffer.pop();
			}
			else
			{
				cout << pq.top() << endl;
				pq.pop();
			}
		}
		else
		{
			while (!buffer.empty())
			{
				pq.push(buffer.front());
				buffer.pop();
			}
		}
	}
}