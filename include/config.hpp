#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include "compact_trie_iterator.hpp"

#include "compact_trie_iv_iterator.hpp"

using namespace std;
/*
    Current Iterator should be set to the iterator that wants to be used
    The class that is assigned to CurrentIterator must implement the Iterator abstract class 
*/
// typedef CompactTrieIterator CurrentIterator;
// typedef CompactTrie CTrie;

typedef CompactTrieIVIterator CurrentIterator;
typedef CompactTrieIV CTrie;

#endif