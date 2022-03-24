#ifndef TRIE_INTERFACE_H
#define TRIE_INTERFACE_H

#include <iostream>

using namespace std;

class TrieInterface{
    // private:

    public:
        // Iterator(){};
        // Iterator(CompactTrie* ct){};
        virtual ~TrieInterface(){};
        virtual uint64_t size() = 0;
        virtual uint32_t succ0(uint32_t it) = 0;
        virtual uint32_t prev0(uint32_t it) = 0;
        virtual uint32_t child(uint32_t it, uint32_t n) = 0;
        virtual uint32_t childrenCount(uint32_t it) = 0;
        virtual uint32_t getPosInParent(uint32_t it) = 0;
        virtual uint32_t childRank(uint32_t it) = 0;
        virtual uint32_t parent(uint32_t it) = 0;
        virtual uint32_t key_at(uint32_t it) = 0;
        virtual void storeToFile(string file_name) = 0;
        virtual void loadFromFile(string file_name) = 0;
};

#endif