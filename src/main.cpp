#include <iostream>
#include <sdsl/vectors.hpp>
#include <sdsl/rank_support.hpp>
#include <sdsl/select_support.hpp>
#include "compact_trie_iterator.hpp" 
#include "table_indexing.hpp"
// #include "wavelet_tree.hpp"
#include <vector>

using namespace std;
using namespace sdsl;

typedef unsigned int u_int;
int main(int argc, char** argv)
{  
    try
    {
        if(argc <= 1){
            cout<<"No extra command line argument passed other that program name"<<endl;
            return 0;
        }  
        string file_name;
        file_name = argv[1];

        TableIndexer ti = TableIndexer();

        ti.indexNewTable(file_name);
        }
    catch(const char* msg)
    {
        cerr<<msg<<endl;
    }
    
    

    // CompactTrieIterator cti = CompactTrieIterator();
    // cti.load_from_file();


    // //bit vector dfuds
    // // bit_vector B = {1,1,0,1,1,0,1,1,1,0,1,1,0,0,0,1,1,1,0,0,0,0,1,0,0,1,0,1,0,0};   
    // //              0 1 2 3 4 5 6 7 8 9
    // // vector<u_int> S = {0, 0, 1, 3, 3, 4, 5, 4, 5, 6, 8, 9, 2, 5, 2}; 
    // bit_vector B = {1,0,1,1,0,1,1,1,0,1,0,1,1,0,1,1,1,0,1,0,1,0,0,0,0,0,0,0,0}; //louds
    // //string dfuds
    // // string s = "1 3 3 4 5 4 5 6 8 9 2 5 2";
    // string s = "1 3 3 4 5 5 4 5 6 8 9 2 2"; //louds

    // CompactTrieIterator cti = CompactTrieIterator(B, s);
    // // CompactTrie ct = CompactTrie(B,S);

    // string instr;

    // while(true){
    //     cin>>instr;

    //     try{
    //         if(instr=="open")cti.open();
    //         if(instr=="next")cti.next();
    //         if(instr=="end")cout<<cti.atEnd()<<endl;
    //         if(instr=="key")cout<<cti.key()<<endl;
    //         // if(instr=="show")ct.showValues();
    //         if(instr=="up")cti.up();
    //         if(instr=="seek"){
    //             int x;
    //             cin>>x;
    //             cti.seek(x);
    //         }
    //         if(instr=="close")break;
    //     }
    //     catch(const char* msg){
    //         cerr<<msg<<endl;
    //     }
    // }
}