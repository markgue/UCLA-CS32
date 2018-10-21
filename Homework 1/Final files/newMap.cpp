#include "newMap.h"
#include <iostream>

Map::Map()
{
	size_ = 0;
	maxSize_ = DEFAULT_MAX_ITEMS;
	pairs_ = new MapPair[maxSize_];
}

Map::Map(int maxsize)
{
	size_ = 0;
	
	if (maxsize > 0)
		maxSize_ = maxsize;
	else
		maxSize_ = DEFAULT_MAX_ITEMS;

	pairs_ = new MapPair[maxSize_];
}

Map::Map(const Map &src)
{
	size_ = src.size_;
	maxSize_ = src.maxSize_;
	pairs_ = new MapPair[maxSize_];
	for (int i = 0; i < size_; i++)
		pairs_[i] = src.pairs_[i];
}

Map &Map::operator= (const Map &src)
{
	if (&src == this)
		return (*this);
	delete[] pairs_;
	size_ = src.size_;
	maxSize_ = src.maxSize_;
	pairs_ = new MapPair[maxSize_];
	for (int i = 0; i < size_; i++)
		pairs_[i] = src.pairs_[i];
	return(*this);
}

Map::~Map()
{
	delete [] pairs_;
}

// Return true if the map is empty, otherwise false.
bool Map::empty() const
{
	return (size_ == 0);
}

// Return the number of key/value pairs in the map.
int Map::size() const
{
	return size_;
}

// If key is not equal to any key currently in the map, and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that either the key is already in the map, or the map has a fixed
// capacity and is full).
bool Map::insert(const KeyType& key, const ValueType& value)
{
	// Check if the array is full
	if (size() >= maxSize_)
	{
		return false;
	}

	// Check if the key has been used yet
	for (int i = 0; i < size(); i++)
	{
		if (pairs_[i].key == key)
		{
			return false;
		}
	}

	// Insert the new key
	pairs_[size_].key = key;
	pairs_[size_].value = value;
	size_++;
	return true;
}

// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// Otherwise, make no change to the map and return false.
bool Map::update(const KeyType& key, const ValueType& value)
{
	for (int i = 0; i < size(); i++)
	{
		if (pairs_[i].key == key)
		{
			pairs_[i].value = value;
			return true;
		}
	}
	return false;
}

// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// If key is not equal to any key currently in the map and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that the key is not already in the map and the map has a fixed
// capacity and is full).
bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
	if (update(key, value))
		return true;
	return insert(key, value);
}

// If key is equal to a key currently in the map, remove the key/value
// pair with that key from the map and return true.  Otherwise, make
// no change to the map and return false.
bool Map::erase(const KeyType& key)
{
	for (int i = 0; i < size(); i++)
	{
		if (pairs_[i].key == key)
		{
			for (int j = i; j < size() - 1; j++)
			{
				pairs_[j] = pairs_[j + 1];
			}
			size_--;
			return true;
		}
	}
	return false;
}

// Return true if key is equal to a key currently in the map, otherwise
// false.
bool Map::contains(const KeyType& key) const
{
	for (int i = 0; i < size(); i++)
	{
		if (pairs_[i].key == key)
			return true;
	}
	return false;
}

// If key is equal to a key currently in the map, set value to the
// value in the map that that key maps to, and return true.  Otherwise,
// make no change to the value parameter of this function and return
// false.
bool Map::get(const KeyType& key, ValueType& value) const
{
	for (int i = 0; i < size(); i++)
	{
		if (pairs_[i].key == key)
		{
			value = pairs_[i].value;
			return true;
		}
	}
	return false;
}

// If 0 <= i < size(), copy into the key and value parameters the
// key and value of one of the key/value pairs in the map and return
// true.  Otherwise, leave the key and value parameters unchanged and
// return false.  (See below for details about this function.)
bool Map::get(int i, KeyType& key, ValueType& value) const
{
	if (i >= 0 && i < size())
	{
		key = pairs_[i].key;
		value = pairs_[i].value;
		return true;
	}
	return false;
}


// Exchange the contents of this map with the other one.
void Map::swap(Map& other)
{
	//swap arrays
	MapPair *tempptr = pairs_;
	pairs_ = other.pairs_;
	other.pairs_ = tempptr;

	//swap sizes
	int tempsize = size_;
	size_ = other.size_;
	other.size_ = tempsize;

	//swap max size values
	tempsize = maxSize_;
	maxSize_ = other.maxSize_;
	other.maxSize_ = tempsize;
}

void Map::dump() const
{
	for (int i = 0; i < size(); i++)
	{
		std::cerr << pairs_[i].key << ", " << pairs_[i].value << std::endl;
	}
}