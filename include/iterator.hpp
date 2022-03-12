#ifndef ITERATOR_H
#define ITERATOR_H

#include <iostream>
// #include "compact_trie.hpp"
#include "trie_interface.hpp"

using namespace std;

class Iterator{
    private:

    public:
        // Iterator(){};
        // Iterator(CompactTrie* ct){};
        virtual ~Iterator(){};
        virtual bool atEnd() = 0;
        virtual uint64_t key() = 0;
        virtual void seek(uint64_t seekKey) = 0;
        virtual void next() = 0;
        virtual void open() = 0;
        virtual void up() = 0;
        virtual int get_depth() = 0;
        // virtual void storeToFile(string file_name) = 0;
        // virtual void loadFromFile(string file_name) = 0;
        virtual void backToStart() = 0;
        bool operator < (Iterator& it) {
            return (key() < it.key());
        }
        //Temporal
        virtual void getIteratorPos() = 0;
        virtual TrieInterface* getCompactTrie() = 0;
};

#endif