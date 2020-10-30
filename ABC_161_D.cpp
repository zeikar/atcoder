#include <iostream>
#include <algorithm>
using namespace std;

int num[20];

int main() {
	int k;
	cin >> k;

	num[0] = 1;
	int digits = 1;

	for (int i = 1; i < k; i++)
	{
		// 뒷자리부터 체크
		bool changed = false;
		for (int j = digits - 1; j >= 0; j--)
		{
			if ((j == 0 || num[j - 1] >= num[j]) && num[j] != 9)
			{
				num[j] ++;
				changed = true;

				// 뒷자리는 1 감소하게 설정
				for (int k = j + 1; k < digits; k++)
				{
					num[k] = max(0, num[k - 1] - 1);
				}

				break;
			}
		}

		// 전부 999999... 임
		if (changed == false)
		{
			// 100000 ... 으로 변환
			++digits;
			num[0] = 1;
			for (int j = 1; j < digits; j++)
			{
				num[j] = 0;
			}
		}
	}

	// debug
	for (int j = 0; j < digits; j++)
	{
		cout << num[j];
	}
	cout << endl;
}