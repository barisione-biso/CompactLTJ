#ifndef REGULAR_TRIE_H
#define REGULAR_TRIE_H

#include <iostream>
// #include <unordered_set>
#include <unordered_map>

using namespace std;

class Trie{
    private:
        unordered_map<u_int64_t, Trie*> children;
        unordered_map<u_int64_t, Trie*>::iterator it;
        // unordered_set<u_int64_t> tags;
        // unordered_set<Trie*> children;
        // unordered_set<u_int64_t>::const_iterator it;
        // unordered_set<Trie*>::const_iterator node_it;

    public:

        Trie(){
            cout<<"creando el Trie"<<endl;
        }

        Trie* insert(u_int64_t);
};

/*
    Creates a new node in the trie if the tags wasn't already in the trie
*/
Trie* Trie::insert(u_int64_t tag){
    it = children.find(tag);
    Trie* node;
    if(it == children.end()){
        node = new Trie;
        children[tag] = node;
    }
    else{
        node = children[tag];
    }
    return node;
}

#endif