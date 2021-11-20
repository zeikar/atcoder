#include <iostream>
using namespace std;

int main() {
	int s, t, x;

	cin >> s >> t >> x;

	if ((s < t && (s <= x && x < t)) || (s > t && (x >= s || x < t)))
	{
		cout << "Yes" << endl;
	}
	else
	{
		cout << "No" << endl;
	}
}