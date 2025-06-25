/**
 * @file anagram_dict.cpp
 * Implementation of the AnagramDict class.
 *
 * @author Matt Joras
 * @date Winter 2013
 */

#include "anagram_dict.h"
#include <algorithm>
#include <fstream>

using std::string;
using std::vector;
using std::ifstream;

// Helper function to get the sorted version of a word
string getSortedWord(const string& word) {
    string sorted = word;
    std::sort(sorted.begin(), sorted.end());
    return sorted;
}

AnagramDict::AnagramDict(const string& filename)
{
    ifstream wordsFile(filename);
    string word;
    while (getline(wordsFile, word)) {
        string sort = getSortedWord(word);
        dict[sort].push_back(word);
    }
}

AnagramDict::AnagramDict(const vector<string>& words)
{
    for (const string& w : words) {
        string sort = getSortedWord(w);
        dict[sort].push_back(w);
    }
}

vector<string> AnagramDict::get_anagrams(const string& word) const
{
    string sort = getSortedWord(word);
    auto w = dict.find(sort);
    if (w != dict.end()) {
        return w->second;
    }
    return vector<string>();
}

vector<vector<string>> AnagramDict::get_all_anagrams() const
{
    vector<vector<string>> ret;
    for (const auto& a : dict) {
        if (a.second.size() > 1) {
            ret.push_back(a.second);
        }
    }
    return ret;
}
