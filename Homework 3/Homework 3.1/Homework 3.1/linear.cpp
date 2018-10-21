// somePredicate returns true if x is true and false if x is false 
bool somePredicate(double x)
{
	return (bool)x == true;
}

bool allTrue(const double a[], int n)
{
	if (n <= 0)
		return true;
	// n >= 1
	if (somePredicate(a[n - 1]))
		return allTrue(a, n - 1);
	else
		return false;
}


int countFalse(const double a[], int n)
{
	if (n <= 0)
		return 0;
	// n >= 1
	return !somePredicate(a[n - 1]) + countFalse(a, n - 1);
}

int firstFalse(const double a[], int n)
{
	if (n <= 0)
		return -1;
	// Check the first element of the array
	if (somePredicate(a[0]) == false)
		return 0;
	int x = firstFalse(a + 1, n - 1);
	if (x == -1)
		return x;
	else
		return x + 1;
}

int indexOfMin(const double a[], int n)
{
	if (n <= 0)
		return -1;
	if (n == 1)
		return 0;
	// compute the minimum of the first n-1 elements
	int x = indexOfMin(a, n - 1);
	// Find the minimum of that value and the nth value
	if (a[x] <= a[n - 1])
		return x;
	else
		return n - 1;
}

bool includes(const double a1[], int n1, const double a2[], int n2)
{
	if (n1 < n2 || n1 < 0)
		return false;
	if (n2 == 0)
		return true;
	if (a1[0] == a2[0])
		return includes(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
	else
		return includes(a1 + 1, n1 - 1, a2, n2);
}



//#include <iostream>
//#include <cassert>
//using namespace std;
//
//int main()
//{
//	double s1[4] = { 10, 20, 30, 40 };
//	double s2[5] = { 10, 0, -1, 5015.2, 1235 };
//	double s3[2] = { 10, 30 };
//	double s4[10] = { 0, 0, 0, 0, 0, 800, 0, 0, -10, 0 };
//
//	cout << allTrue(s1, 4) << allTrue(s2, 5) << endl;
//	cout << countFalse(s1, 4) << " " << countFalse(s2, 5) << " " << countFalse(s4, 10) << endl;
//	cout << firstFalse(s1, 4) << " " << firstFalse(s2, 5) << " " << firstFalse(s4, 10) << endl;
//	cout << indexOfMin(s1, 4) << " " << indexOfMin(s2, 5) << " " << indexOfMin(s4, 10) << endl;
//	cout << includes(s1, 4, s3, 2) << endl;
//}