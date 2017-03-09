#ifndef TRIEAPI
#define TRIEAPI

#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

typedef struct NodeStruct
{
    bool validNgram = false;
    unordered_map<string, struct NodeStruct*> children;
    struct NodeStruct* parent;
    string nodeWord;

    NodeStruct(struct NodeStruct* par, string w) : parent(par), nodeWord(w) {} //constructor (yeah, structs have them too)
    NodeStruct() {}
}Node;


void addNgramTrie(Node* root, string ngram);
void findNgramsTrie(Node* root, string query);
void splitText(string query, vector<string> &words);
void deleteNgramTrie(Node* root, string ngram);
void recursiveParentDeletion(Node* node, Node *root);


#endif // TRIEAPI

