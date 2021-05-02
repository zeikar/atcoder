#include <iostream>
#include <deque>
#include <string>
using namespace std;

int main() {
	string s;
	bool reversed = false;
	deque<char> deq;

	cin >> s;

	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] == 'R')
		{
			reversed = !reversed;
		}
		else
		{
			if (reversed)
			{
				deq.push_front(s[i]);
			}
			else
			{
				deq.push_back(s[i]);
			}
		}
	}

	string res = "";

	while (!deq.empty())
	{
		if (reversed)
		{
			res += deq.back();
			deq.pop_back();
		}
		else
		{
			res += deq.front();
			deq.pop_front();
		}
	}

	deq.clear();
	for (int i = 0; i < res.length(); i++)
	{
		if (!deq.empty() && deq.back() == res[i])
		{
			deq.pop_back();
		}
		else
		{
			deq.push_back(res[i]);
		}
	}

	string result = "";
	while (!deq.empty())
	{
		cout << deq.front();
		deq.pop_front();
	}
	cout << endl;
}