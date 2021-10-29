#include <iostream>
#include <string>
using namespace std;

int main() {
	string s;
	int k;
	cin >> s >> k;

	for (int i = 0; i < s.size(); i++)
	{
		if (s[i] == 'a')
		{
			continue;
		}

		int diff = 26 - (s[i] - 'a');
		// possible to change to a
		if (k >= diff)
		{
			s[i] = 'a';
			k -= diff;
		}
	}

	// remains
	s[s.size() - 1] = ((s[s.size() - 1] - 'a') + k % 26) % 26 + 'a';

	cout << s << endl;
}