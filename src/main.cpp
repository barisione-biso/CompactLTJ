#include <iostream>
#include <sdsl/vectors.hpp>
#include <sdsl/rank_support.hpp>
#include <sdsl/select_support.hpp>
#include "compact_trie_iterator.hpp" 
#include "table_indexing.hpp"
#include "compact_trie.hpp"
// #include "wavelet_tree.hpp"
#include <vector>
#include "utils.hpp"

using namespace std;
using namespace sdsl;

// pair<uint64_t, uint64_t> binary_search_seek(uint64_t val, uint64_t i, uint64_t f, int_vector<> &v){
//     if(v[f]<val)return make_pair(0,f+1);
//     uint64_t mid; 
//     while(i<f){
//         mid = (i + f)/2;
//         if(v[mid]<val)i = mid+1;
//         else if(v[mid]>=val)f = mid;
//     }
//     return make_pair(v[i], i);
// }

// typedef unsigned int u_int;

// uint64_t getMaxDataPoint(string file_name){
//     ifstream reader(file_name);
//     string line;
//     bool first_line = true;
//     bool second_line = false;
//     u_int64_t maximum = 0;

//     while(reader.is_open() && getline(reader, line)){
//         if(first_line && line.substr(0,4) == "dim:"){
//             first_line = false;
//             second_line = true;
//         }
//         else if(second_line && line.substr(0, 7) == "orders:"){
//             second_line = false;
//         }
//         else{
//             vector<string> line_values = parse(line, ' ');

//             for(int i=0; i<3; i++){
//                 uint64_t val =  stoi(line_values[i]);
//                 if(val > maximum){
//                     maximum = val;
//                 }
//             }
//         }
//     }
//     reader.close();

//     return maximum;
// }

void maxValueIndexChecking(string file_name){
    // TableIndexer ti = TableIndexer();
    // Index index = ti.loadIndex(file_name);

    // vector<string> orders = index.getOrders();
    // for(auto order : orders){
    //     CTrie ct = index.getTrie(order);
    //     cout<<"Max value for sequence in order "<<order<<": "<<ct.getMaxSequence()<<endl;
    // }
}

int main(int argc, char** argv){
    // int_vector<> vec = {1, 2, 3, 5, 6, 7};
    // pair<uint64_t, uint64_t> info = binary_search_seek(1, 0, 0, vec);
    // cout<<info.first<<" "<<info.second<<endl;
    // cout<<getMaxDataPoint(argv[1])<<endl;
    // maxValueIndexChecking(argv[1]);
}
// int main(int argc, char** argv)
// {  
//     bit_vector B = {1,0,1,1,0,1,1,1,0,1,1,0,1,1,0,1,1,1,0,1,0,1,1,0,1,0,0,0,0,0,0,0,0,0,0};
//     string s = "1 3 1 2 3 2 3 5 7 4 6 7 2 2 3 6";

//     CompactTrie* ct = new CompactTrie(B, s);
//     CompactTrieIterator cti = CompactTrieIterator(ct); //0
//     CompactTrieIterator cti2 = CompactTrieIterator(ct); //1
//     string instr;
//     bool op;

//     while(true){
//         cin>>instr>>op;

//         try{
//             if(instr=="open") op? cti2.open() : cti.open();
//             if(instr=="next")op? cti2.next() : cti.next();
//             if(instr=="end")op? cout<<cti2.atEnd()<<endl : cout<<cti.atEnd()<<endl;
//             if(instr=="key")op? cout<<cti2.key()<<endl : cout<<cti.key()<<endl;
//             // if(instr=="show")ct.showValues();
//             if(instr=="up")op? cti2.up() : cti.up();
//             if(instr=="seek"){
//                 int x;
//                 cin>>x;
//                 op? cti2.seek(x) : cti.seek(x);
//             }
//             if(instr=="close")break;
//         }
//         catch(const char* msg){
//             cerr<<msg<<endl;
//         }
//     }
    
// }