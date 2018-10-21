// Return the number of ways that all n2 elements of a2 appear
// in the n1 element array a1 in the same order (though not
// necessarily consecutively).  The empty sequence appears in a
// sequence of length n1 in 1 way, even if n1 is 0.
// For example, if a1 is the 7 element array
//	10 50 40 20 50 40 30
// then for this value of a2     the function must return
//	10 20 40			1
//	10 40 30			2
//	20 10 40			0
//	50 40 30			3
int countIncludes(const double a1[], int n1, const double a2[], int n2)
{
	// n1 is smaller than n2 or n1 is empty, so return 0
	if (n1 < n2 || n1 < 0)
		return 0;
	// n2 is empty, so it has been fully accounted for and can return 1
	if (n2 == 0)
		return 1;
	// Check for equal first values, indicating the start of a sequence
	int count = 0;
	if (a1[0] == a2[0])
		count = countIncludes(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
	// Also check if the next array element in a1 could have a match with a2[0]
	return countIncludes(a1 + 1, n1 - 1, a2, n2) + count;
}

// Exchange two doubles
void exchange(double& x, double& y)
{
	double t = x;
	x = y;
	y = t;
}

// Rearrange the elements of the array so that all the elements
// whose value is > splitter come before all the other elements,
// and all the elements whose value is < splitter come after all
// the other elements.  Upon return, firstNotGreater is set to the
// index of the first element in the rearranged array that is
// <= splitter, or n if there is no such element, and firstLess is
// set to the index of the first element that is < splitter, or n
// if there is no such element.
// In other words, upon return from the function, the array is a
// permutation of its original value such that
//   * for 0 <= i < firstNotGreater, a[i] > splitter
//   * for firstNotGreater <= i < firstLess, a[i] == splitter
//   * for firstLess <= i < n, a[i] < splitter
// All the elements > splitter end up in no particular order.
// All the elements < splitter end up in no particular order.
void split(double a[], int n, double splitter,
	int& firstNotGreater, int& firstLess)
{
	if (n < 0)
		n = 0;

	// It will always be the case that just before evaluating the loop
	// condition:
	//  firstNotGreater <= firstUnknown and firstUnknown <= firstLess
	//  Every element earlier than position firstNotGreater is > splitter
	//  Every element from position firstNotGreater to firstUnknown-1 is
	//    == splitter
	//  Every element from firstUnknown to firstLess-1 is not known yet
	//  Every element at position firstLess or later is < splitter

	firstNotGreater = 0;
	firstLess = n;
	int firstUnknown = 0;
	while (firstUnknown < firstLess)
	{
		if (a[firstUnknown] < splitter)
		{
			firstLess--;
			exchange(a[firstUnknown], a[firstLess]);
		}
		else
		{
			if (a[firstUnknown] > splitter)
			{
				exchange(a[firstNotGreater], a[firstUnknown]);
				firstNotGreater++;
			}
			firstUnknown++;
		}
	}
}

// Rearrange the elements of the array so that
// a[0] >= a[1] >= a[2] >= ... >= a[n-2] >= a[n-1]
// If n <= 1, do nothing.
void order(double a[], int n)
{
	if (n <= 1)
		return;
	int firstNotGreater, firstLess;
	split(a, n, a[0], firstNotGreater, firstLess);
	order(a, firstNotGreater + 1);
	order(a + firstLess, n - firstLess);
}

//#include <iostream>
//#include <cassert>
//using namespace std;
//
//int main()
//{
//	double s1[6] = { 10, 20, 30, 40, 30, 30 };
//	double s2[5] = { 10, 0, -1, 5015.2, 1235 };
//	double s3[2] = { 10, 30 };
//	double s4[4] = { 10, 10, 30, 30 };
//	double s5[10] = { 5, 10, 15, 20, 25, 30, 10, 30, 10, 30 };
//
//	cout << countIncludes(s1, 4, s3, 2) << endl; // 1
//	cout << countIncludes(s1, 6, s3, 2) << endl; // 3
//	cout << countIncludes(s2, 4, s3, 2) << endl; // 0
//	cout << countIncludes(s4, 4, s3, 2) << endl; // 4
//	cout << countIncludes(s5, 10, s3, 2) << endl; // 6
//	cout << countIncludes(s5, 10, s3, 0) << endl; // 1
//
//	for (int i = 0; i < 10; i++)
//		cout << s5[i] << " ";
//	cout << endl;
//
//	order(s5, 10);
//	
//	for (int i = 0; i < 10; i++)
//		cout << s5[i] << " ";
//	cout << endl;
//}