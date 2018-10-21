#include "provided.h"
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class DecrypterImpl
{
public:
    bool load(string filename);
    vector<string> crack(const string& ciphertext);
private:
	// Private member functions
	string Crack(string ciphertext_message, vector<string>& output);

	// Private member variables
	WordList words_;
	Translator translator_;
};

bool DecrypterImpl::load(string filename)
{
	return words_.loadWordList(filename);
}

vector<string> DecrypterImpl::crack(const string& ciphertext)
{
	vector<string> result;
	Crack(ciphertext, result);
	sort(result.begin(), result.end());
	return result; 
}

string DecrypterImpl::Crack(string ciphertext_message, vector<string>& output)
{
	// Tokenize the cipher text
	Tokenizer tokenizer(" 0123456789,;:.!()[]{}-\"#$%^&");
	vector<string> tokens = tokenizer.tokenize(ciphertext_message);
	
	// Find the word with the most untranslated text
	unsigned int mostUntranslatedIndex = 0;
	int mostUntranslatedLength = 0;
	for (unsigned int i = 0; i < tokens.size(); i++)
	{
		string word = tokens[i];
		string translatedWord = translator_.getTranslation(word);
		int currentUntranslated = 0;
		for (int j = 0; j < translatedWord.length(); j++)
		{
			if (translatedWord[j] == '?')
				currentUntranslated++;
		}
		if (currentUntranslated >= mostUntranslatedLength)
		{
			mostUntranslatedLength = currentUntranslated;
			mostUntranslatedIndex = i;
		}
	}
	
	// Take the most untranslated word and attempt to solve with it
	
	string chosenWord = tokens[mostUntranslatedIndex];
	string chosenWordTranslation = translator_.getTranslation(chosenWord);
	vector<string> candidates = words_.findCandidates(chosenWord, chosenWordTranslation);

	// For each candidate
	for (unsigned int i = 0; i < candidates.size(); i++)
	{
		// If the map is valid
		if (translator_.pushMapping(chosenWord, candidates[i]))
		{
			// Using the current mapping, split everything into two vectors: complete words and incomplete words
			string translation = translator_.getTranslation(ciphertext_message);
			vector<string> candidateTokens = tokenizer.tokenize(translation);
			vector<string> completeWords;
			bool hasIncompleteWords = false;
			for (unsigned int j = 0; j < candidateTokens.size(); j++)
			{
				string str = candidateTokens[j];
				bool isComplete = true;
				for (int k = 0; k < str.length(); k++)
				{
					if (str[k] == '?')
					{
						isComplete = false;
						hasIncompleteWords = true;
						break;
					}
				}
				if (isComplete)
					completeWords.push_back(str);
			}

			// Check the complete words vector to ensure everything exists
			bool validMap = true;
			for (unsigned int j = 0; j < completeWords.size(); j++)
			{
				// If some complete word is not in the word list
				if (!words_.contains(completeWords[j]))
				{
					// remove the mapping and try something else
					translator_.popMapping();
					validMap = false;
					break;
				}
			}
			
			if (validMap)
			{
				if (!hasIncompleteWords)
				{
					output.push_back(translation);
				}
				else
				{
					Crack(ciphertext_message, output);
				}
				translator_.popMapping();
			}

		} // end if valid mapping
	} // Loop over each candidate
	
	return "";
}




//******************** Decrypter functions ************************************

// These functions simply delegate to DecrypterImpl's functions.
// You probably don't want to change any of this code.

Decrypter::Decrypter()
{
    m_impl = new DecrypterImpl;
}

Decrypter::~Decrypter()
{
    delete m_impl;
}

bool Decrypter::load(string filename)
{
    return m_impl->load(filename);
}

vector<string> Decrypter::crack(const string& ciphertext)
{
   return m_impl->crack(ciphertext);
}
