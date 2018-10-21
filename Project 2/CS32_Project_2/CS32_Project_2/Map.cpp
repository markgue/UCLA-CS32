#include "Map.h"
#include <iostream>

using namespace std;

// Default constructor
// Sets head and tail to nullptr
Map::Map()
{
	head = nullptr;
	tail = nullptr;
}

// Copy constructor
Map::Map(const Map & src)
{
	head = nullptr;
	tail = nullptr;

	// Go through the list back to front, inserting each node into the new map
	// Since nodes are inserted at the front, this copies the source list order
	Node *p = src.tail;
	while (p != nullptr)
	{
		insert(p->key, p->value);
		p = p->prev;
	}
}

// Assignment operator
Map & Map::operator=(const Map & src)
{
	// Do nothing if assigning a map to itself
	if (&src == this)
		return *this;
	
	// Delete everything in the existing linked list
	clear();

	// Now that the Map is empty, fill it with the values in the src Map
	// (Same method as the copy constructor)
	Node *p = src.tail;
	while (p != nullptr)
	{
		insert(p->key, p->value);
		p = p->prev;
	}
	return *this;
}

// Destructor
Map::~Map()
{
	clear();
}


// Member functions				//
//								//
//								//

bool Map::empty() const
{
	return head == nullptr;
}

int Map::size() const
{
	int count = 0;
	Node *p = head;
	while (p != nullptr)
	{
		p = p->next;
		count++;
	}
	return count;
}

bool Map::insert(const KeyType & key, const ValueType & value)
{
	// Inserts new nodes onto the front of the of the linked list
	
	// First searches the list to see if the key value has been used
	if (contains(key))
		return false;
	
	// Since the key value has not been used yet, allocate a new node and insert it onto the front of the list
	Node *n = new Node;

	// Fill the map pair data
	n->key = key;
	n->value = value;

	// Assign the pointers
	n->prev = nullptr;
	n->next = head;
	if (head != nullptr)
		head->prev = n;
	head = n;
	
	// If the list was empty beforehand, make the tail point to the new object
	if (tail == nullptr)
		tail = n;

	return true;
}

bool Map::update(const KeyType & key, const ValueType & value)
{
	// Searches the list for the key and changes the matching value if the key is found
	Node *p = findKey(key);
	if (p != nullptr)
	{
		p->value = value;
		return true;
	}
	// Since the key was not found, return false
	return false;
}

bool Map::insertOrUpdate(const KeyType & key, const ValueType & value)
{	
	// Update portion:
	// Searches the list for the key and changes the matching value if the key is found
	Node *p = findKey(key);
	if (p != nullptr)
	{
		p->value = value;
		return true;
	}
	// Insert portion:
	// Since the key value has not been used yet, allocate a new node and insert it onto the front of the list
	Node *n = new Node;

	// Fill the map pair data
	n->key = key;
	n->value = value;

	// Assign the pointers
	n->prev = nullptr;
	n->next = head;
	if (head != nullptr)
		head->prev = n;
	head = n;

	// If the list was empty beforehand, make the tail point to the new object
	if (tail == nullptr)
		tail = n;

	return true;
}

bool Map::erase(const KeyType & key)
{
	Node *p = findKey(key);
	if (p != nullptr)
	{
		// Update head and/or tail
		if (p == head)
			head = p->next;
		if (p == tail)
			tail = p->prev;
			
		// Check if there is a node after and change its prev value
		if (p->next != nullptr)
			p->next->prev = p->prev;
		// Check if there is a node before and change its prev value
		if (p->prev != nullptr)
			p->prev->next = p->next;
		// Note: works for beginning and ending nodes as adjacent nodes will have their prev/next pointers set to nullptr since p prev/next points to nullptr on the head/tail node
			
		//delete the node and return true
		delete p;
		return true;
	}
	// The key was not found; return false
	return false;
}

bool Map::contains(const KeyType & key) const
{
	if (findKey(key) == nullptr)
		return false;
	return true;
}

bool Map::get(const KeyType & key, ValueType & value) const
{
	Node *p = findKey(key);
	if (p != nullptr)
	{
		value = p->value;
		return true;
	}
	return false;

}

bool Map::get(int i, KeyType & key, ValueType & value) const
{
	if (i < 0 || i >= size())
		return false;
	Node *p = head;
	for (int k = 0; k < i; k++)
	{
		p = p->next;
	}
	key = p->key;
	value = p->value;
	return true;
}

void Map::swap(Map & other)
{
	Node *temp;
	// Exchange head values
	temp = head;
	head = other.head;
	other.head = temp;
	// Exchange tail values
	temp = tail;
	tail = other.tail;
	other.tail = temp;
}

void Map::dump() const
{
	Node *p = head;
	while (p != nullptr)
	{
		cerr << p->key << " " << p->value << endl;
		p = p->next;
	}
	cerr << endl;
}

// Private member functions		//
//								//
//								//

// findKey searches the list for a given key and returns a pointer to that node if found, otherwise it returns nullptr
Map::Node* Map::findKey(const KeyType& key) const
{
	Node *p = head;
	while (p != nullptr)
	{
		if (p->key == key)
			return p;
		p = p->next;
	}
	return nullptr;
}

// clear deletes all nodes in the linked list and sets head and tail to nullptr
void Map::clear()
{
	Node *p = head;
	while (p != nullptr)
	{
		p = p->next;
		delete head;
		head = p;
	}
	head = nullptr;
	tail = nullptr;
}


// Non-member functions			//
//								//
//								//

bool combine(const Map& m1, const Map& m2, Map& result)
{
	// Bool to keep track of what to return (true if there were no mismatching key-value pairs in m1 and m2)
	bool noMismatch = true;

	// Variables used in loops
	KeyType tempKey;
	ValueType tempValue;

	// Create a temporary empty map to fill with values
	Map tempResult;
	
	// Loop through m1's list
	for (int i = 0; i < m1.size(); i++)
	{
		// Get the map pair data at i 
		m1.get(i, tempKey, tempValue);
		// Check that the key is not already in m2, and if not insert the pair into result
		if (m2.contains(tempKey))
		{
			ValueType matchingKeyValue;
			m2.get(tempKey, matchingKeyValue);
			if (tempValue != matchingKeyValue) 
			{
				noMismatch = false;
				continue;
			}
		}
		tempResult.insert(tempKey, tempValue);
	}

	// Loop through m2's list
	for (int i = 0; i < m2.size(); i++)
	{
		m2.get(i, tempKey, tempValue);
		// Since any values with matching keys are already tested for matching values when looping through m1,
		// we can simply insert key in m2 that is not already in m1
		// The insert function will not place in pairs that already have an existing key
		if (!m1.contains(tempKey))
			tempResult.insert(tempKey, tempValue);
	}
	
	// Swap the data in result and tempResult
	// once the function ends tempResult will be cleared
	result.swap(tempResult);
	return noMismatch;
}

void subtract(const Map& m1, const Map& m2, Map& result)
{
	// Variables used in loops
	KeyType tempKey;
	ValueType tempValue;

	// Create a temporary empty map to fill with values
	Map tempResult;

	// Loop through m1's list
	for (int i = 0; i < m1.size(); i++)
	{
		// Get the map pair data at i 
		m1.get(i, tempKey, tempValue);
		// Check that the key is not already in m2, and if not insert the pair into result
		if (!m2.contains(tempKey))
			tempResult.insert(tempKey, tempValue);
	}
	// Swap the data in result and tempResult
	// once the function ends tempResult will be cleared
	result.swap(tempResult);
}