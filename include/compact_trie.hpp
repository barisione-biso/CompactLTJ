#ifndef COMPACT_TRIE_H
#define COMPACT_TRIE_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sdsl/vectors.hpp>
#include <sdsl/wavelet_trees.hpp>
#include <sdsl/wm_int.hpp>
#include "utils.hpp"

using namespace std;
using namespace sdsl;

class CompactTrie{
    private:
        bit_vector B;
        // wm_int<> wt;

        /*
            Initializes rank and select support for B
        */
       void initializeSupport(){
            util::init_support(b_rank1,&B);
            util::init_support(b_rank0,&B);
            util::init_support(b_sel1,&B);
            util::init_support(b_sel0,&B);
       }

       // Evaluar si después es mejor recibir los tags como ints
       int_vector<> turn_into_int_vector(string s){
           //Parsear string por espacios
           vector<string> values = parse(s, ' ');
           //// Crear int_vector para resultados int_vector<> O(n+1);
           int_vector<> tags(values.size());
           
           //convertir cada termino parseado en un entero y guardarlo en el vector
           int i = 0;
           for(auto v: values){
               tags[i++] = stoi(v);
           }
           return tags;
       }

    public:

        //Rank & Support arrays
        rank_support_v<1> b_rank1;
        rank_support_v<0> b_rank0;
        select_support_mcl<0> b_sel0;
        select_support_mcl<1> b_sel1;

        wm_int<bit_vector> wt;

        /*
            Constuctor from LOUDS representation (b) and tags for trie representations (s)
        */
        CompactTrie(bit_vector b, string s){
            B = b;
            construct_im(wt, turn_into_int_vector(s));
            initializeSupport();
        }

        /*
            Constructor from file with representation
        */
        CompactTrie(string file_name){
            loadFromFile(file_name);
        };

        /*
            Destructor
        */
        ~CompactTrie(){};

        /*
            Recives index in bit vector
            Returns index of next 0
        */
        uint64_t succ0(uint64_t it){
            uint64_t cant_0 = b_rank0(it);
            return b_sel0(cant_0 + 1);
        }
        
        /*
            Recives index in bit vector
            Returns index of previous 0
        */
        uint64_t prev0(uint64_t it){
            uint64_t cant_0 = b_rank0(it);
            return b_sel0(cant_0);
        }

        /*
            Recives index of current node and the child that is required
            Returns index of the nth child of current node
        */
        uint64_t child(uint64_t it, uint64_t n){
            return b_sel0(b_rank1(it+n)) + 1;
        }

        /*
            Recives index of node whos children we want to count
            Returns how many children said node has
        */
        uint64_t childrenCount(uint64_t it){
            return succ0(it) - it;
        }

        /*
            Recives node index
            Returns index of position in parent
        */
        uint64_t getPosInParent(uint64_t it){
            return b_sel1(b_rank0(it));
        }

        /*
            Recives index of node
            Return which child of its parent it is
        */
        uint64_t childRank(uint64_t it){
            uint64_t pos = getPosInParent(it);
            return pos - prev0(pos);
        }

        /*
            Recives index of node
            Returns index of parent node
        */  
        uint64_t parent(uint64_t it){
            uint64_t pos = getPosInParent(it);
            return prev0(pos) + 1;
        }

        /*
            Returns key that corresponds to given node(it)
        */
        uint64_t key_at(uint64_t it){
            return wt[b_rank0(it)-2];
        }

        /*
            Returns i-th element of the original sequence s 
        */
        uint64_t get_wt_at(uint64_t i){
            return wt[i];
        }

        
        /*
            Stores Compact Trie Iterator to file saving the size of B, B and S.
        */
        void storeToFile(string file_name){
            string B_file = file_name + ".B";
            string WM_file = file_name + ".WM";
            store_to_file(B, B_file);
            store_to_file(wt, WM_file);
        }

        /*
            Loads Compact Trie from file restoring B and the Wavelet Tree
        */
        void loadFromFile(string file_name){
            string B_file = file_name + ".B";
            string WM_file = file_name + ".WM";
            load_from_file(B, B_file);
            load_from_file(wt, WM_file);
            initializeSupport();
        }
};

#endif