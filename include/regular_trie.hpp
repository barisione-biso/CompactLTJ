#ifndef REGULAR_TRIE_H
#define REGULAR_TRIE_H

#include <iostream>
#include <map> 

using namespace std;

class Trie{
    private:
        map<u_int64_t, Trie*> children;
        map<u_int64_t, Trie*>::iterator it;
        bool has_children;
    public:
    
        Trie(){
            has_children = false;
        }

        Trie* insert(u_int64_t);
        void traverse();
        bool hasChildren();
};

/*
    Creates a new node in the trie if the tag wasn't already in the trie
*/
Trie* Trie::insert(u_int64_t tag){
    has_children = true;
    it = children.find(tag);
    Trie* node;
    if(it == children.end()){
        node = new Trie();
        children[tag] = node;
    }
    else{
        node = children[tag];
    }
    return node;
}

/*
    Traverses trie in preorder printer tags of children on each node
*/
void Trie::traverse(){
    if(has_children){
        it = children.begin();
        while(it!=children.end()){
            cout<< it->first <<" ";
            it++;
        }
        cout<<endl;

        it = children.begin();
        
        while(it != children.end()){
            it->second->traverse();
            it++;
        }
    }
}

/*
    Returns true if the node has children
*/
bool Trie::hasChildren(){
    return has_children;
}

#endif