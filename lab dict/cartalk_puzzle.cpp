/**
 * @file cartalk_puzzle.cpp
 * Holds the function which solves a CarTalk puzzler.
 *
 * @author Matt Joras
 * @date Winter 2013
 */

#include <fstream>
#include <iterator>
#include <algorithm>
#include <cctype>

#include "cartalk_puzzle.h"

using namespace std;

/**
 * Solves the CarTalk puzzler described here:
 * http://www.cartalk.com/content/wordplay-anyone.
 * @return A vector of (string, string, string) tuples
 * Returns an empty vector if no solutions are found.
 * @param d The PronounceDict to be used to solve the puzzle.
 * @param word_list_fname The filename of the word list to be used.
 */
vector<std::tuple<std::string, std::string, std::string>> cartalk_puzzle(PronounceDict d,
                                                                         const string &word_list_fname)
{
    vector<std::tuple<std::string, std::string, std::string>> ret;

    /* Your code goes here! */
    ifstream word_file(word_list_fname);
    string word;

    while (word_file >> word) {
        if (word.length() == 5) {
            string word_ = word;
            transform(word_.begin(), word_.end(), word_.begin(), ::toupper);
            string word1 = word_.substr(1);
            string word2 = word_.substr(0, 1) + word_.substr(2);
 
            if (d.homophones(word1, word_) && d.homophones(word2, word_) && d.homophones(word1, word2)) {

                string one = word.substr(1);
                string two = word.substr(0, 1) + word.substr(2);
                
                ret.push_back(make_tuple(word, one, two));
            }
        }
    }

    return ret;
}
