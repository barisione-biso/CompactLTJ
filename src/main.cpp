#include <iostream>
#include <sdsl/vectors.hpp>
#include <sdsl/rank_support.hpp>
#include <sdsl/select_support.hpp>
#include "compact_trie_iterator.hpp" 
#include "table_indexing.hpp"
#include "compact_trie.hpp"
// #include "wavelet_tree.hpp"
#include <vector>

using namespace std;
using namespace sdsl;

typedef unsigned int u_int;
int main(int argc, char** argv)
{  
    bit_vector B = {1,0,1,1,0,1,1,1,0,1,1,0,1,1,0,1,1,1,0,1,0,1,1,0,1,0,0,0,0,0,0,0,0,0,0};
    string s = "1 3 1 2 3 2 3 5 7 4 6 7 2 2 3 6";

    // CompactTrie* ct = new CompactTrie(B, s);
    // CompactTrieIterator cti = CompactTrieIterator(ct); //0
    // CompactTrieIterator cti2 = CompactTrieIterator(ct); //1
    // string instr;
    // bool op;

    // while(true){
    //     cin>>instr>>op;

    //     try{
    //         if(instr=="open") op? cti2.open() : cti.open();
    //         if(instr=="next")op? cti2.next() : cti.next();
    //         if(instr=="end")op? cout<<cti2.atEnd()<<endl : cout<<cti.atEnd()<<endl;
    //         if(instr=="key")op? cout<<cti2.key()<<endl : cout<<cti.key()<<endl;
    //         // if(instr=="show")ct.showValues();
    //         if(instr=="up")op? cti2.up() : cti.up();
    //         if(instr=="seek"){
    //             int x;
    //             cin>>x;
    //             op? cti2.seek(x) : cti.seek(x);
    //         }
    //         if(instr=="close")break;
    //     }
    //     catch(const char* msg){
    //         cerr<<msg<<endl;
    //     }
    // }
    
}