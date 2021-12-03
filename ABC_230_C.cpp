#include <iostream>
#include <string>
using namespace std;

int main() {
	long long n, a, b;

	cin >> n >> a >> b;

	long long p, q, r, s;

	cin >> p >> q >> r >> s;

	long long k1 = max(1 - a, 1 - b);
	long long k2 = max(1 - a, b - n);

	long long x1 = a + k1;
	long long y1 = b + k1;
	long long x2 = a + k2;
	long long y2 = b - k2;

	for (long long i = p; i <= q; i++)
	{
		for (long long j = r; j <= s; j++)
		{
			// line 
			if (i - j == x1 - y1)
			{
				cout << '#';
			}
			else if (i + j == x2 + y2)
			{

				cout << '#';
			}
			else
			{
				cout << '.';
			}
		}
		cout << endl;
	}
	cout << endl;
}