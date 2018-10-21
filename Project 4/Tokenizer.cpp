#include "provided.h"
#include <string>
#include <vector>
#include <set>
using namespace std;

class TokenizerImpl
{
public:
    TokenizerImpl(string separators);
    vector<string> tokenize(const std::string& s) const;
private:
	set<char> separatorChars;
};

TokenizerImpl::TokenizerImpl(string separators)
{
	for (unsigned int i = 0; i < separators.size(); i++)
	{
		separatorChars.insert(separators[i]);
	}
}

vector<string> TokenizerImpl::tokenize(const std::string& s) const
{
	vector<string> v;
	string token = "";
	for (unsigned int i = 0; i < s.size(); i++)
	{
		// Check if the character does not match one of the separators
		if (separatorChars.find(s[i]) == separatorChars.end())
		{
			token += s[i];
		}
		else if (token != "")
		{
			v.push_back(token);
			token = "";
		}
	}
	if (token != "")
		v.push_back(token);
	return v;
}

//******************** Tokenizer functions ************************************

// These functions simply delegate to TokenizerImpl's functions.
// You probably don't want to change any of this code.

Tokenizer::Tokenizer(string separators)
{
    m_impl = new TokenizerImpl(separators);
}

Tokenizer::~Tokenizer()
{
    delete m_impl;
}

vector<string> Tokenizer::tokenize(const std::string& s) const
{
    return m_impl->tokenize(s);
}
