#include <iostream>
#include <set>
#include <algorithm>
#include <map>
#include <string>
#include "trieapi.h"

using namespace std;
inline void readInitialInput();
inline void deleteNgram(string ngram);
inline void addNgram(string ngram);
inline void runQuery(string query);

Node trie;

int main()
{
    std::ios::sync_with_stdio(false);
    readInitialInput();
    string line;
    while (getline(cin, line))
    {
        switch (line[0])
        {
            case 'A':
                addNgram(line.substr(2));
                break;
            case 'D':
                deleteNgram(line.substr(2));
                break;
            case 'Q':
                runQuery(" " + line.substr(2) + " ");
                break;
            case 'F':
                continue;
        }
    }
}

/**
 * Adds ngrams(seperated by \n) to ngrams set
 * until it reaches the S character and after
 * prints R
 */
inline void readInitialInput()
{
    string line;

    while(getline(cin,line) && line[0] != 'S')
    {
        addNgramTrie(&trie, line);
    }
    cout << "R" << endl;
}

/**
 * Adds the ngram to ngrams set
 * @param ngram
 */
inline void addNgram(const string ngram)
{
    addNgramTrie(&trie, ngram);
}

/**
 * Marks matched ngrams with "\0"
 * //Deletes all ngrams match the given string from vector
 * @param ngram
 */
inline void deleteNgram(const string ngram)
{
    deleteNgramTrie(&trie, ngram);
}

/**
 * Executes the given query and prints the result
 * Stores all matched ngrams into results multimap
 * (like map but you can use same index for different value)
 * Sorted automaticly with O(log(n)) (balanced binary tree)
 * by index value first and size later
 * @param query
 */
inline void runQuery(const string query)
{
    findNgramsTrie(&trie, query);
}





