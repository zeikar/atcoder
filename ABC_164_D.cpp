#include <iostream>
#include <string>
using namespace std;

int main() {
	string s;
	cin >> s;

    int mult = 1;
    int sum = 0;
    int cnt[2019] = { 1 };
    long long ans = 0;
    
    for (int i = s.size() - 1; i >= 0; i--) {
        sum += mult * (s[i] - '0');
        sum %= 2019;

        mult *= 10;
        mult %= 2019;

        ans += cnt[sum]++;
    }

    cout << ans << endl;
}