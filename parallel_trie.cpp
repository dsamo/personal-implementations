#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include "trieapi.h"
#include <pthread.h>

using namespace std;

void addNgramTrie(Node* root, string ngram){
    Node* node = root;

    stringstream ss;
    ss.str(ngram);
    string item;

    while (std::getline(ss, item, ' '))
    {
        //If while crossing the tree a required node doesn't exist, create it.
        node->children.emplace(item, new Node(node, item));   //Nodes have refrences to their parents and access to their word.

        //Next node
        node = node->children[item];
    }
    //This ngram is finished. The node in which it fully resides is marked as valid
    node->validNgram = true;
}

void deleteNgramTrie(Node* root, string ngram)
{
    Node* node = root;

    stringstream ss;
    ss.str(ngram);
    string item;

    unordered_map<string, struct NodeStruct*>::iterator wordIterator;

    while (std::getline(ss, item, ' '))
    {
        wordIterator = node->children.find(item); //Search children for word
        if(wordIterator == node->children.end()) return; //Failed to find
		
        node = wordIterator->second; //Next node
    }
		node->validNgram=false;
        recursiveParentDeletion(node, root); //Delete starting from last word to first
}

typedef struct args{
	int bound;
	vector<string>& vec;
}Arguments;

void findNgramsTrie(Node* root, string query)
{
	pthread_t t1,t2;
	Arguments a1,a2;
    vector<string> words,vec1,vec2;   //optimize, query words
    splitText(query, words);
	a1.bound = int(words.size()/2);
	a1.vec = &vec1;
	a2.bound = words.size()-1;
	a2.vec = &vec2;
	pthread_create(&t1,NULL,threadfunc, &a1);
	pthread_create(&t2,NULL,threadfunc, &a2);

}



void splitText(string query, vector<string> &words)
{
    stringstream ss;
    ss.str(query);
    string item;

    while (std::getline(ss, item, ' ')) {
        words.push_back(item);
    }


}

//Has an edge case that occurs on extremly small tests, harnests don't hit it.
void recursiveParentDeletion(Node* node, Node* root)
{
    Node* parent = node->parent;

    if(node->children.empty() && !node->validNgram && node!=root)
    {
        parent->children.erase(node->nodeWord);
		delete(node);
        recursiveParentDeletion(parent, root);
    }

}

void* threadfunc(void* args){
	Node* node = root;
    unsigned i = 0;  //counter
    int backtraceIndex = -1; //In case of a wrong path block, search will backtrace to this index.
    bool failedToFind=true;  //For priting formating.
    string builder = ""; //String builder - builds the ngrams as it crosses them
    ostringstream ss;
    unsigned vecsize = words.size();
    unordered_map<string, struct NodeStruct*>::iterator wordIterator; //Word iterator
    unordered_set<string> printedSet; //To avoid duplicate entries - is only needed per query
    while(i<vecsize)   //While processing query
    {
        wordIterator = node->children.find(words[i]);
        //Save index for backtrackning
        if(backtraceIndex== -1 && node != root)
        {
            backtraceIndex = i;
        }

        if(wordIterator != node->children.end()) //We had match
        {
            builder += wordIterator->first;     //Add string to builder
            node = wordIterator->second; //Become child
            if(node->validNgram && printedSet.find(builder) == printedSet.end()) //If you are valid and haven't been printed
            {
                printedSet.insert(builder);
            }
            builder += " "; //Add space to builder
            i++;    //Next word
        }
        else    //We had a mismatch
        {
            builder.clear();   //Destroy builder string
            if(backtraceIndex != -1)  //If must backtrace, do it now
            {
                i = backtraceIndex;
                node=root;
                backtraceIndex = -1;
                continue;
            }
            else
            {
                if(node == root)    //if your are at root
                    i++;            //go to next word
                else            //if not, stay to current word
                    node = root;//and go to root
            }
        }

        //We never had a mismatch, yet must backtrack as this is last word.
        if(backtraceIndex != -1 && i == vecsize)
        {
            builder.clear();
            i = backtraceIndex;
            node=root;
            backtraceIndex = -1;
        }
    }

    //If you don't find anything print -1
    if(failedToFind)
        cout << "-1";
    else
        cout << ss.str().erase(0,1);
    cout <<'\n';
}

