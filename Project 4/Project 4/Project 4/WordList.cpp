#include "provided.h"
#include "MyHash.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

class WordListImpl
{
public:
    bool loadWordList(string filename);
    bool contains(string word) const;
    vector<string> findCandidates(string cipherWord, string currTranslation) const;
private:
	string getLetterPattern(string str) const;
	MyHash<string, vector<string>> wordMap;
};

bool WordListImpl::loadWordList(string filename)
{
	wordMap.reset();
	ifstream infile(filename);
	// Check if the file is opened
	if (!infile)
		return false;

	string s;
	while (getline(infile, s))
	{
		for (int i = 0; i < s.length(); i++)
		{
			s[i] = tolower(s[i]);
		}
		// Get the pattern for the key
		string pattern = getLetterPattern(s);
		// Search for the key and if found, push s into the existing vector
		vector<string>* wordVector = wordMap.find(pattern);
		if (wordVector != nullptr)
		{
			wordVector->push_back(s);
		}
		// Otherwise there is no corresponding key, so create a new vector and associate the key to it
		else
		{
			vector<string> sVector;
			sVector.push_back(s);
			wordMap.associate(pattern, sVector);
		}
	}
	return true;
}

bool WordListImpl::contains(string word) const
{
	// convert to lower case
	string lowercaseword = word;
	for (int i = 0; i < lowercaseword.length(); i++)
	{
		lowercaseword[i] = tolower(lowercaseword[i]);
	}
	// get the pattern of the lowercase word
	string pattern = getLetterPattern(lowercaseword);
	// find the vector it would be contained in and try to find the word
	const vector<string>* wordVector = wordMap.find(pattern);
	if (wordVector == nullptr)
		return false;
	for (unsigned int i = 0; i < wordVector->size(); i++)
	{
		if ((*wordVector)[i] == lowercaseword)
			return true;
	}
	return false;
}

vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const
{
	vector<string> candidates;	

	// Check for valid inputs
	if (cipherWord.length() != currTranslation.length())
		return candidates;  
	for (unsigned int i = 0; i < cipherWord.length(); i++)
	{
		char ch = tolower(cipherWord[i]);
		// if ch is not ' or a letter
		if ( !(ch == '\'' || (ch >= 'a' && ch <= 'z')) )
			return candidates;
		// if ch2 is not ', ?, or a letter
		char ch2 = tolower(currTranslation[i]);
		if ( !(ch2 == '?' || ch2 == '\'' || (ch2 >= 'a' && ch2 <= 'z')) )
			return candidates;
	}
	// Get a vector of all words that match the pattern of the cipherword
	string pattern = getLetterPattern(cipherWord);
	const vector<string>* patternWords = wordMap.find(pattern);
	// For each matching pattern word
	for (unsigned int i = 0; i < patternWords->size(); i++)
	{
		string word = (*patternWords)[i];
		bool isCandidate = true;
		// Check if the characters that are not ? match with the characters in the current translation
		for (int j = 0; j < currTranslation.length(); j++)
		{
			if (currTranslation[j] != '?')
			{
				if (tolower(currTranslation[j]) != tolower(word[j]))
				{
					isCandidate = false;
					break;
				}
			}
		}
		// If all of them matched add the word to the vector
		if (isCandidate)
		{
			candidates.push_back(word);
		}
	}
	// Return the vector of valid candidates
	return candidates;
}

string WordListImpl::getLetterPattern(string str) const
{
	// Create an array of corresponding characters (shouldn't need more than 26 + 1 for ', but make it larger just in case)
	string letterPattern;
	char patterns[30][2];
	int letterCount = 0;
	for (unsigned int i = 0; i < str.size(); i++)
	{
		bool repeated = false;
		for (int j = 0; j < letterCount; j++)
		{
			if (patterns[j][0] == tolower(str[i]))
			{
				letterPattern += patterns[j][1];
				repeated = true;
				continue;
			}
		}
		if (!repeated)
		{
			patterns[letterCount][0] = tolower(str[i]);
			if (str[i] == '\'')
			{
				patterns[letterCount][0] = '\'';
				patterns[letterCount][1] = '\'';
			}
			else
			{
				patterns[letterCount][1] = 'A' + letterCount;
			}
			letterPattern += patterns[letterCount][1];
			letterCount++;
		}
	}
	return letterPattern;
}

//***** hash functions for string, int, and char *****

unsigned int hasher(const std::string& s)
{
	return std::hash<std::string>()(s);
}

unsigned int hasher(const int& i)
{
	return std::hash<int>()(i);
}

unsigned int hasher(const char& c)
{
	return std::hash<char>()(c);
}

//******************** WordList functions ************************************

// These functions simply delegate to WordListImpl's functions.
// You probably don't want to change any of this code.

WordList::WordList()
{
    m_impl = new WordListImpl;
}

WordList::~WordList()
{
    delete m_impl;
}

bool WordList::loadWordList(string filename)
{
    return m_impl->loadWordList(filename);
}

bool WordList::contains(string word) const
{
    return m_impl->contains(word);
}

vector<string> WordList::findCandidates(string cipherWord, string currTranslation) const
{
   return m_impl->findCandidates(cipherWord, currTranslation);
}