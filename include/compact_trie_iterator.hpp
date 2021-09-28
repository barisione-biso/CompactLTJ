#ifndef COMPACT_TRIE_ITERATOR_H
#define COMPACT_TRIE_ITERATOR_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sdsl/vectors.hpp>
#include <sdsl/wavelet_trees.hpp>
#include "iterator.hpp"

using namespace std;
using namespace sdsl;

class CompactTrieIterator: public Iterator{
    private:
        bool at_end;
        bool at_root;
        uint64_t it;
        uint64_t parent_it;
        uint64_t pos_in_parent;
        // string file_name;
        //Louds representation to save tree structure
        bit_vector B;
        //Wavelet tree to save tree keys
        wt_int<> wt;

        rank_support_v<1> b_rank1; //ocupado
        rank_support_v<0> b_rank0; //ocupado
        select_support_mcl<0> b_sel0; //ocupado
        select_support_mcl<1> b_sel1; //ocupado

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
            Initializes rank and select support for B
        */
       void initializeSupport(){
            util::init_support(b_rank1,&B);
            util::init_support(b_rank0,&B);
            util::init_support(b_sel1,&B);
            util::init_support(b_sel0,&B);
       }

    public:

        /*
            Constructor for initializing from file 
        */
        CompactTrieIterator(string file_name){
            loadFromFile(file_name);
        };

        /*
            Constructor for initializing from table
        */
        CompactTrieIterator(bit_vector b, string s){
            B = b;
            it = 2;
            construct_im(wt, s, 'd');
            at_root = true;
            at_end = false;
            // file_name = "order1.txt";
            initializeSupport();
        }

        /*
            Destructor
        */
        ~CompactTrieIterator(){};

        /*
            Returns the key of the current position of the iterator
        */
        uint64_t key(){
            if(at_end){
                throw "Iterator is atEnd";
            }
            else if(at_root){
                throw "Root doesnt have key";
            }
            else{
                return wt[b_rank0(it)-2];
            }
        }

        /*
            Returns true if the iterator is past the last child of a node
        */
        bool atEnd(){
            return at_end;
        }

        /*
            Moves the iterator to the first key on the next level
        */
        void open(){

            if(at_root){
                at_root = false;
                
            }
            if(at_end){
                throw "Iterator is atEnd";
            }
            else{
                parent_it = it;
                it = child(it, 1);
                pos_in_parent = 1;
            }
        }

        /*
            Moves the iterator to the next key
        */
        void next(){
            if(at_root){
                throw "At root, doesn't have next";
            }
            if(at_end){
                throw "Iterator is atEnd";
            }

            uint64_t parent_child_count = childrenCount(parent_it);
            if(parent_child_count == pos_in_parent){
                at_end = true;
            }
            else{
                pos_in_parent++;
                it = child(parent_it, pos_in_parent);
            }
        }
        
        /*
            Moves the iterator to the parent of the current node
        */
        void up(){
            if(at_root){
                throw "At root, cant go up";
            }

            it = parent_it;
            at_end = false;
            if(it==2){
                at_root = true;
            }
            else{
                pos_in_parent = childRank(it);
                parent_it = parent(it);
            }
        }

        /*
            Moves the iterator to the closes position that is equal or bigger than seek key
            If not possible then it moves the iterator to the end
        */
        void seek(uint64_t seek_key){
            if(at_root){
                throw "At root, cant seek";
            }
            if(at_end){
                throw "At end, cant seek";
            }

            uint64_t parent_child_count = childrenCount(parent_it);
            uint64_t i = b_rank0(it)-2;
            uint64_t f = b_rank0(child(parent_it, parent_child_count))-2;
            
            bool found = false;
            for(i=i; i<=f; i++){
                if(wt[i]>=seek_key){
                    it = b_sel0(i+2)+1;
                    pos_in_parent = childRank(it);
                    found = true;
                    break;
                }
            }

            if(!found){
                at_end = true;
            }
        }

        /*
            Stores Compact Trie Iterator to file saving the size of B, B and S.
        */
        void storeToFile(string file_name){
            ofstream stream(file_name);
            if(stream.is_open()){
                stream<<B.size()<<'\n';
                for(uint64_t i=0; i<B.size(); i++){
                    stream<<B[i]<<" ";
                }
                stream<<'\n';
                for(uint64_t i=0; i<wt.size(); i++){
                    stream<<wt[i]<<" ";
                }
            }
            stream.close();
        }

        /*
            Loads Compact Trie from file restoring B and the Wavelet Tree
        */
        void loadFromFile(string file_name){
            ifstream stream(file_name);
            uint64_t B_size;
            string s;
            uint64_t val;
            at_root = true;
            at_end = false;
            it = 2;

            if(stream.is_open()){
                stream>>B_size;
                B = bit_vector(B_size);
                for(uint64_t i=0; i<B_size; i++){
                    stream>>val;
                    B[i] = val;
                }
                stream.ignore(numeric_limits<streamsize>::max(),'\n');
                getline(stream, s);
            }
            stream.close();
            
            construct_im(wt, s, 'd');
            initializeSupport();
        }
};

#endif