// https://www.geeksforgeeks.org/meet-in-the-middle/

// C++ program to demonstrate working of Meet in the 
// Middle algorithm for maximum subset sum problem. 
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long int ll;
ll X[1 << 21], Y[1 << 21];

// Find all possible sum of elements of a[] and store 
// in x[] 
void calcsubarray(ll a[], ll x[], int n, int c)
{
	for (int i = 0; i < (1 << n); i++)
	{
		ll s = 0;
		for (int j = 0; j < n; j++)
			if (i & (1 << j))
				s += a[j + c];
		x[i] = s;
	}
}

// Returns the maximum possible sum less or equal to S 
ll solveSubsetSum(ll a[], int n, ll S)
{
	// Compute all subset sums of first and second 
	// halves 
	calcsubarray(a, X, n / 2, 0);
	calcsubarray(a, Y, n - n / 2, n / 2);

	int size_X = 1 << (n / 2);
	int size_Y = 1 << (n - n / 2);

	// Sort Y (we need to do doing binary search in it) 
	sort(Y, Y + size_Y);

	// To keep track of the maximum sum of a subset 
	// such that the maximum sum is less than S 
	ll max = 0;

	// Traverse all elements of X and do Binary Search 
	// for a pair in Y with maximum sum less than S. 
	for (int i = 0; i < size_X; i++)
	{
		if (X[i] <= S)
		{
			// lower_bound() returns the first address 
			// which has value greater than or equal to 
			// S-X[i]. 
			int p = lower_bound(Y, Y + size_Y, S - X[i]) - Y;

			// If S-X[i] was not in array Y then decrease 
			// p by 1 
			if (p == size_Y || Y[p] != (S - X[i]))
				p--;

			if ((Y[p] + X[i]) > max)
				max = Y[p] + X[i];
		}
	}
	return max;
}

// Driver code 
int main()
{
	int n, t;
	ll a[40];

	cin >> n >> t;
	for (int i = 0; i < n; i++)
	{
		cin >> a[i];
	}

	cout << solveSubsetSum(a, n, t) << endl;
}