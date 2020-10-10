#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>
using namespace std;

int h, w;
string arr[100];

int putMat1(int i, int j) {
	// put 2 horiz
	for (int k = 0; k < 2; k++)
	{
		if (i + k >= h)
		{
			return 0;
		}

		if (arr[i + k][j] == '#')
		{
			return 0;
		}
	}

	return 1;
}

int putMat2(int i, int j) {
	// put 2 horiz
	for (int k = 0; k < 2; k++)
	{
		if (j + k >= w)
		{
			return 0;
		}

		if (arr[i][j + k] == '#')
		{
			return 0;
		}
	}

	return 1;
}


int main() {
	int result = 0;
	cin >> h >> w;

	for (int i = 0; i < h; i++)
	{
		cin >> arr[i];
	}

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			result += putMat1(i, j);
			result += putMat2(i, j);
		}
	}

	cout << result << endl;
}