#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
	long long a, b, c, d;
	vector<long long> ans;

	cin >> a >> b >> c >> d;

	ans.push_back(a * c);
	ans.push_back(a * d);
	ans.push_back(b * c);
	ans.push_back(b * d);

	sort(ans.begin(), ans.end());
	

	cout << ans[3] << endl;
}