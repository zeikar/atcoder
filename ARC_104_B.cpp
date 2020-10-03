#include <iostream>
#include <string>
using namespace std;

int main() {
	int n;
	string s;

	int cnta = 0, cntt = 0, cntg = 0, cntc = 0;
	int answer = 0;

	cin >> n >> s;

	for (int i = 0; i < s.size(); i++)
	{
		// cnt
		if (s[i] == 'A')
		{
			cnta++;
		}
		else if (s[i] == 'T')
		{
			cntt++;
		}
		else if (s[i] == 'C')
		{
			cntc++;
		}
		else
		{
			cntg++;
		}

		int mcnta = 0, mcntt = 0, mcntc = 0, mcntg = 0;
		for (int j = 0; j <= i; j++)
		{

			int a = cnta - mcnta;
			int t = cntt - mcntt;
			int c = cntc - mcntc;
			int g = cntg - mcntg;

			//cout << cnta << mcnta << cntt << mcntt << cntc << mcntc << cntg << mcntg << endl;

			// check
			if (a == t && c == g)
			{
				//cout << i << ' ' << j << endl;

				++answer;
			}


			// cnt
			if (s[j] == 'A')
			{
				mcnta++;
			}
			else if (s[j] == 'T')
			{
				mcntt++;
			}
			else if (s[j] == 'C')
			{
				mcntc++;
			}
			else
			{
				mcntg++;
			}
		}
	}

	cout << answer << endl;
}