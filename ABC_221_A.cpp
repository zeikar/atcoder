#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

int main() {
	int a, b;

	cin >> a >> b;

	cout << (1 << (5 * (a- b))) << endl;
}