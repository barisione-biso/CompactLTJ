#ifndef TRIE_INTERFACE_H
#define TRIE_INTERFACE_H

#include <iostream>

using namespace std;

class TrieInterface{
    private:

    public:
        // Iterator(){};
        // Iterator(CompactTrie* ct){};
        virtual ~TrieInterface(){};
        virtual uint64_t size() = 0;
        virtual uint64_t succ0(uint64_t it) = 0;
        virtual uint64_t prev0(uint64_t it) = 0;
        virtual uint64_t child(uint64_t it, uint64_t n) = 0;
        virtual uint64_t childrenCount(uint64_t it) = 0;
        virtual uint64_t getPosInParent(uint64_t it) = 0;
        virtual uint64_t childRank(uint64_t it) = 0;
        virtual uint64_t parent(uint64_t it) = 0;
        virtual uint64_t key_at(uint64_t it) = 0;
        virtual void storeToFile(string file_name) = 0;
        virtual void loadFromFile(string file_name) = 0;
};

#endif