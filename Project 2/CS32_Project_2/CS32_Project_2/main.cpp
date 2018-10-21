#include "Map.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	// All tests conducted using KeyType std::string and ValueType double
	
	// Create two maps with the default constructor
	// Map b will remain empty for testing member functions
	Map a, b;
	// Test size and empty
	assert(a.empty());
	assert(a.size() == 0);
	
	// Test insert
	a.insert("ABC", 100);
	a.insert("DEF", 200);
	assert(!a.insert("ABC", 500));
	
	// Test size and empty again
	assert(!a.empty());
	assert(a.size() == 2);
	
	// Test update and get
	KeyType key;
	ValueType value;
	assert(a.update("ABC", 400));
	assert(!a.update("test", 200));
	assert(!b.update("test", 200));
	assert(a.get("ABC", value) && value == 400);
	assert(a.get(1, key, value));
	assert(!a.get(2, key, value));
	assert(!b.get("ABC", value));
	assert(!b.get(0, key, value));
	
	// Test insertOrUpdate and contains
	assert(a.insertOrUpdate("GHI", 300));
	assert(a.contains("GHI"));
	assert(!a.contains("GHH"));
	assert(a.insertOrUpdate("GHI", 500));
	assert(a.get("GHI", value) && value == 500);
	assert(!b.contains("ABC"));

	// Test erase
	assert(a.erase("ABC"));
	assert(!a.erase("test"));

	// Test assignment operator and copy constructor
	assert(a.size() == 2);
	assert(a.contains("DEF") && a.contains("GHI"));
	Map c(a);
	assert(a.contains("DEF") && a.contains("GHI"));
	assert(c.contains("DEF") && c.contains("GHI"));
	assert(c.size() == 2);
	Map d = a;
	assert(a.contains("DEF") && a.contains("GHI"));
	assert(d.contains("DEF") && d.contains("GHI"));
	assert(d.size() == 2);

	// Test swap
	Map e;
	e.insert("abc", 50);
	e.insert("def", 20);
	e.insert("hij", 25);
	e.swap(a);
	assert(e.contains("DEF") && e.contains("GHI"));
	assert(e.size() == 2);
	assert(a.contains("abc") && a.contains("def") && a.contains("hij"));
	assert(a.size() == 3);

	// Test combine
	Map a1, a2, a3;
	a1.insert("a", 1);
	a1.insert("b", 2);
	a2.insert("a", 1);
	// Test combine taking multiple of the same input
	// Also tests combining two Maps with the same key-value pairs
	assert(combine(a1, a1, a1));
	assert(combine(a1, a2, a1));
	assert(a1.size() == 2);
	a2.insert("b", 3);
	assert(!combine(a1, a2, a3));
	// a3 should only contain ("a", 1)
	assert(a3.contains("a") && a3.size() == 1);
	Map empty;
	a2 = a3 = empty;
	a3.insert("remove this", 4);
	assert(a2.size() == 0);
	a2.insert("c", 3);
	a2.insert("d", 4);
	assert(combine(a1, a2, a3));
	// Ensure that a1 and a2 combine and a3 does not contain anything it previously did
	assert(a3.size() == 4 && !a3.contains("remove this"));

	// Test subtract
	Map b1, b2, b3;
	b1.insert("a", 1);
	b1.insert("b", 2);
	b1.insert("c", 3);
	subtract(b1, b2, b1);
	assert(b1.contains("a") && b1.contains("b") && b1.contains("c"));
	b2.insert("b", 5);
	subtract(b1, b2, b3);
	assert(b3.contains("a") && b3.contains("c") && b3.size() == 2);
}
