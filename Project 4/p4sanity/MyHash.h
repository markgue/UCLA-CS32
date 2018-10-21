// MyHash.h

// Skeleton for the MyHash class template.  You must implement the first seven
// member functions; we have implemented the eighth.	

#ifndef MYHASH_H
#define MYHASH_H


template <typename KeyType, typename ValueType>
class MyHash
{
public:
    MyHash(double maxLoadFactor = 0.5);
    ~MyHash();
    void reset();
    void associate(const KeyType& key, const ValueType& value);
    int getNumItems() const;
    double getLoadFactor() const;

      // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;

      // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyHash*>(this)->find(key));
    }

      // C++11 syntax for preventing copying and assignment
    MyHash(const MyHash&) = delete;
    MyHash& operator=(const MyHash&) = delete;

private:
	// Private member functions
	unsigned int getBucketNumber(const KeyType& key, int buckets) const;

	// Private struct
	struct PairNode
	{
		PairNode(KeyType key, ValueType val, PairNode* np = nullptr)
		{
			key_ = key; 
			value_ = val;
			next_ = np;
		}
		KeyType key_;
		ValueType value_;
		PairNode* next_;
	};

	// Private data
	int bucketCount_;
	int numItems_;
	double maxLoadFactor_;

	PairNode** hashArray_; 
};



template <typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::MyHash(double maxLoadFactor)
{
	// Create an array of PairNode pointers
	hashArray_ = new PairNode*[100];
	// Fill the pointer array with null pointers
	for (int i = 0; i < 100; i++)
	{
		hashArray_[i] = nullptr;
	}
	// Set the internal value of the bucket count
	bucketCount_ = 100;
	// Set the maximum load factor
	if (maxLoadFactor <= 0)
		maxLoadFactor_ = 0.5;
	else if (maxLoadFactor > 2)
		maxLoadFactor_ = 2.0;
	else
		maxLoadFactor_ = maxLoadFactor;
}

template <typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::~MyHash<KeyType, ValueType>()
{
	// Delete all the dynamically allocated memory in the array
	for (int i = 0; i < bucketCount_; i++)
	{
		PairNode* p = hashArray_[i];
		// Clear any possible linked list at the given pointer
		while (p != nullptr)
		{
			PairNode* temp = p;
			p = p->next_;
			delete temp;
		}
	}
	// Delete the array
	delete [] hashArray_;
}

template <typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::reset()
{
	// Delete all the dynamically allocated memory in the array
	for (int i = 0; i < bucketCount_; i++)
	{
		PairNode* p = hashArray_[i];
		// Clear any possible linked list at the given pointer
		while (p != nullptr)
		{
			PairNode* temp = p;
			p = p->next_;
			delete temp;
		}
	}
	// Clear the array and build a new empty one
	delete [] hashArray_;
	// Create an array of PairNode pointers
	hashArray_ = new PairNode*[100];
	// Fill the pointer array with null pointers
	for (int i = 0; i < 100; i++)
	{
		hashArray_[i] = nullptr;
	}
	// Set the internal value of the bucket count
	bucketCount_ = 100;
	numItems_ = 0;
}

template <typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
	unsigned int keyHash = getBucketNumber(key, bucketCount_);
	// Check if there is already a value in the bucket and add it to that buckets linked list
	if (hashArray_[keyHash] != nullptr)
	{
		PairNode* p = hashArray_[keyHash];
		while (p != nullptr)
		{
			if (p->key_ == key)
			{
				p->value_ = value;
				// If the key already exists, the size of the hash map will not increase, so return
				return;
			}
			if (p->next_ == nullptr)
			{
				p->next_ = new PairNode(key, value);
				numItems_++;
				break;
			}
			p = p->next_;
		}
	}
	// Otherwise just directly place the new pair into the empty bucket
	else
	{
		hashArray_[keyHash] = new PairNode(key, value);
		numItems_++;
	}
	
	// Check if the new array needs to grow (i.e. the hash exceeds the given load factor)
	if (getLoadFactor() > maxLoadFactor_)
	{
		// Make an array twice the size of the current one
		PairNode** newArray = new PairNode*[bucketCount_ * 2];
		for (int i = 0; i < bucketCount_ * 2; i++)
			newArray[i] = nullptr;

		// Loop through the original array and transfer all of the data while deleting it
		for (int i = 0; i < bucketCount_; i++)
		{
			PairNode* p = hashArray_[i];
			// Clear any possible linked list at the given pointer
			while (p != nullptr)
			{
				PairNode* temp = p;
				p = p->next_;
				
				// Insert into the new array
				unsigned int keyHash = getBucketNumber(temp->key_, bucketCount_ * 2);
				// Check if there is already a value in the bucket and add it to that buckets linked list
				if (newArray[keyHash] != nullptr)
				{
					PairNode* p2 = new PairNode(temp->key_, temp->value_, newArray[keyHash]);
					newArray[keyHash] = p2;
				}
				// Otherwise just directly place the new pair into the empty bucket
				else
				{
					newArray[keyHash] = new PairNode(temp->key_, temp->value_);
				}

				// Remove the node in the original array
				delete temp;
			} // end while
		} // end for loop over original array
		delete[] hashArray_;
		hashArray_ = newArray;
		bucketCount_ = bucketCount_ * 2;
	}
}

template <typename KeyType, typename ValueType>
int MyHash<KeyType, ValueType>::getNumItems() const
{
	return numItems_;
}

template <typename KeyType, typename ValueType>
double MyHash<KeyType, ValueType>::getLoadFactor() const
{
	return (double)numItems_/(double)bucketCount_;
}

template <typename KeyType, typename ValueType>
const ValueType* MyHash<KeyType, ValueType>::find(const KeyType& key) const
{
	int index = getBucketNumber(key, bucketCount_);
	if (hashArray_[index] == nullptr)
		return nullptr;
	PairNode* temp = hashArray_[index];
	while (temp != nullptr)
	{
		if (temp->key_ == key)
			return &temp->value_;
		temp = temp->next_;
	}
	return nullptr;
}

template <typename KeyType, typename ValueType>
unsigned int MyHash<KeyType, ValueType>::getBucketNumber(const KeyType& key, int buckets) const
{
	unsigned int hasher(const KeyType& k);
	unsigned int hashValue = hasher(key);
	return hashValue % buckets;
}

#endif