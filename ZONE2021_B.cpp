#include <iostream>
#include <iomanip>
using namespace std;

int main() {
	int N, D, H;

	cin >> N >> D >> H;

	long  double res = 0;

	for (int i = 0; i < N; i++)
	{
		int d, h;
		cin >> d >> h;

		long double y = (long double)(H - h) / (D - d) * (-d) + h;
		res = max(res, y);
	}

	cout << fixed << setprecision(12) << res << endl;
}