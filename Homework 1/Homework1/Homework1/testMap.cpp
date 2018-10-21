#include "Map.h"

#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	Map m;
	m.insert("A", 10);
	m.insert("B", 44);
	m.insert("C", 10);
	string all;
	double total = 0;
	for (int n = 0; n < m.size(); n++)
	{
		string k;
		double v;
		m.get(n, k, v);
		all += k;
		total += v;
	}

	cout << all << total << endl;

	Map gpas;
	gpas.insert("Fred", 2.956);
	gpas.insert("Ethel", 3.538);
	double v;
	string k1;
	assert(gpas.get(1, k1, v) && (k1 == "Fred" || k1 == "Ethel"));
	string k2;
	assert(gpas.get(1, k2, v) && k2 == k1);

	Map m1;
	m1.insert("Fred", 2.956);
	Map m2;
	m2.insert("Ethel", 3.538);
	m2.insert("Lucy", 2.956);
	m1.swap(m2);
	m1.dump();
	m2.dump();
	assert(m1.size() == 2 && m1.contains("Ethel") && m1.contains("Lucy") &&	m2.size() == 1 && m2.contains("Fred"));
}
