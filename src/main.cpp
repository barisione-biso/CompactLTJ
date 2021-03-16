#include <iostream>
#include <sdsl/vectors.hpp>
#include <sdsl/rank_support.hpp>
#include <sdsl/select_support.hpp>
#include "compact_trie.hpp" 

using namespace std;
using namespace sdsl;

typedef unsigned int u_int;
int main()
{
    bit_vector B = {1,1,0,1,1,0,1,1,1,0,1,1,0,0,0,1,1,1,0,0,0,0,1,0,0,1,0,1,0,0};   
    //              0 1 2 3 4 5 6 7 8 9
    vector<u_int> S = {0, 0, 1, 3, 3, 4, 5, 4, 5, 6, 8, 9, 2, 5, 2};

    CompactTrie ct = CompactTrie(B,S);

    string instr;

    while(true){
        cin>>instr;

        try{
            if(instr=="open")ct.open();
            if(instr=="next")ct.next();
            if(instr=="end")cout<<ct.atEnd()<<endl;
            if(instr=="key")cout<<ct.key()<<endl;
            if(instr=="show")ct.showValues();
            if(instr=="up")ct.up();
            if(instr=="seek"){
                int x;
                cin>>x;
                ct.seek(x);
            }
            if(instr=="close")break;
        }
        catch(const char* msg){
            cerr<<msg<<endl;
        }
    }
}