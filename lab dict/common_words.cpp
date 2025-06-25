/**
 * @file common_words.cpp
 * Implementation of the CommonWords class.
 *
 * @author Zach Widder
 * @date Fall 2014
 */

#include "common_words.h"

#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>

using std::string;
using std::vector;
using std::ifstream;
using std::cout;
using std::endl;
using std::feof;

string remove_punct(const string& str)
{
    string ret;
    std::remove_copy_if(str.begin(), str.end(), std::back_inserter(ret),
                        [](int c) {return std::ispunct(c);});
    return ret;
}

CommonWords::CommonWords(const vector<string>& filenames)
{
    init_file_word_maps(filenames);
    init_common();
}

void CommonWords::init_file_word_maps(const vector<string>& filenames)
{
    file_word_maps.resize(filenames.size());

    for (size_t i = 0; i < filenames.size(); i++) {
        vector<string> words = file_to_vector(filenames[i]);
        for (const string& w : words) {
            file_word_maps[i][w]++;
        }
    }
}

void CommonWords::init_common()
{
    for (const auto& map : file_word_maps) {
        for (const auto& w : map) {
            common[w.first]++;
        }
    }
}

/**
 * @param n The number of times to word has to appear.
 * @return A vector of strings. The vector contains all words that appear
 * in each file >= n times.
 */
vector<string> CommonWords::get_common_words(unsigned int n) const
{
    vector<string> out;
 
    for (const auto& w : common) {
        const string& word = w.first;
        unsigned int count = w.second;

        if (count == file_word_maps.size()) {

            bool exists = true;
            for (const auto& file : file_word_maps) {
                auto f = file.find(word);
                if (f == file.end() || f->second < n) {
                    exists = false;
                    break;
                }
            }
            
            if (exists) {
                out.push_back(word);
            }
        }
    }
    
    return out;
}

/**
 * Takes a filename and transforms it to a vector of all words in that file.
 * @param filename The name of the file that will fill the vector
 */
vector<string> CommonWords::file_to_vector(const string& filename) const
{
    ifstream words(filename);
    vector<string> out;

    if (words.is_open()) {
        std::istream_iterator<string> word_iter(words);
        while (!words.eof()) {
            out.push_back(remove_punct(*word_iter));
            ++word_iter;
        }
    }
    return out;
}