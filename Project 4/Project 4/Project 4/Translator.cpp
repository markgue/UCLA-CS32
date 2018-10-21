#include "provided.h"
#include <string>
using namespace std;

class TranslatorImpl
{
public:
	TranslatorImpl();
	~TranslatorImpl();
    bool pushMapping(string ciphertext, string plaintext);
    bool popMapping();
    string getTranslation(const string& ciphertext) const;
private:
	// Private node struct
	struct MapNode
	{
		MapNode(MapNode* other);
		
		char charMap[26][2];
		MapNode* next;
	};

	// Pointer to linked list
	MapNode* stackHead;
};

TranslatorImpl::TranslatorImpl()
{
	stackHead = new MapNode(nullptr);
}

TranslatorImpl::~TranslatorImpl()
{
	while (stackHead != nullptr)
	{
		MapNode* temp = stackHead;
		stackHead = stackHead->next;
		delete temp;
	}
}

bool TranslatorImpl::pushMapping(string ciphertext, string plaintext)
{
	// Check that the inputs are valid
	if (ciphertext.length() != plaintext.length())
		return false;

	for (int i = 0; i < plaintext.length(); i++)
	{
		if (!isalpha(ciphertext[i]) || !isalpha(plaintext[i]))
			return false;
	}
	// Create an array to keep track of what values have been used
	bool usedLetters[26];
	for (int i = 0; i < 26; i++)
		usedLetters[i] = false;
	for (int i = 0; i < 26; i++)
	{
		char letter = stackHead->charMap[i][1];
		if (letter != '?')
			usedLetters[letter - 'A'] = true;
	}

	// Check that the mapping does not overlap with anything that already exists 
	// (i.e. check if H->R already exists, and if P->R is passed in, then return false)
	for (int i = 0; i < plaintext.length(); i++)
	{
		if (usedLetters[toupper(plaintext[i]) - 'A'] && stackHead->charMap[toupper(ciphertext[i]) - 'A'][1] != toupper(plaintext[i]))
			return false;
	}

	// Make a copy of the current mapping
	MapNode* newMap = new MapNode(stackHead);
	
	for (int i = 0; i < ciphertext.length(); i++)
	{
		// Go through the new map and pair everything together, otherwise return false if a pair already exists that isn't the same pair (i.e. passing A->B when A->B exists)
		if (newMap->charMap[toupper(ciphertext[i]) - 'A'][1] == '?' || newMap->charMap[toupper(ciphertext[i]) - 'A'][1] == toupper(plaintext[i]))
		{
			newMap->charMap[toupper(ciphertext[i]) - 'A'][1] = toupper(plaintext[i]);
		}
		else
		{
			delete newMap;
			return false;
		}
	}

	// Make newMap the front of the linked list stack
	newMap->next = stackHead;
	stackHead = newMap;
	return true;
}

bool TranslatorImpl::popMapping()
{
	// Check if the stack only has the array of ?s
	if (stackHead->next == nullptr)
		return false;

	MapNode* temp = stackHead;
	stackHead = stackHead->next;
	delete temp;
	return true;
}

string TranslatorImpl::getTranslation(const string& ciphertext) const
{
	string translation;
	for (unsigned int i = 0; i < ciphertext.length(); i++)
	{
		if (isalpha(ciphertext[i]))
		{
			char ch = stackHead->charMap[toupper(ciphertext[i]) - 'A'][1];
			if (islower(ciphertext[i]))
				translation += tolower(ch);
			else
				translation += ch;
		}
		else
			translation += ciphertext[i];
	}
	return translation; 
}

TranslatorImpl::MapNode::MapNode(MapNode* other)
{
	if (other != nullptr)
	{
		for (int i = 0; i < 26; i++)
		{
			charMap[i][0] = 'A' + i;
			charMap[i][1] = other->charMap[i][1];
		}
	} 
	else
	{
		for (int i = 0; i < 26; i++)
		{
			charMap[i][0] = 'A' + i;
			charMap[i][1] = '?';
		}
	}
	next = nullptr;
}

//******************** Translator functions ************************************

// These functions simply delegate to TranslatorImpl's functions.	
// You probably don't want to change any of this code.

Translator::Translator()
{
    m_impl = new TranslatorImpl;
}

Translator::~Translator()
{
    delete m_impl;
}

bool Translator::pushMapping(string ciphertext, string plaintext)
{
    return m_impl->pushMapping(ciphertext, plaintext);
}

bool Translator::popMapping()
{
    return m_impl->popMapping();
}

string Translator::getTranslation(const string& ciphertext) const
{
    return m_impl->getTranslation(ciphertext);
}
