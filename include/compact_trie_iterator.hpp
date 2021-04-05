#ifndef COMPACT_TRIE_ITERATOR_H
#define COMPACT_TRIE_ITERATOR_H

#include <sdsl/vectors.hpp>
#include <vector>
#include <iostream>
#include <sdsl/wavelet_trees.hpp>

using namespace std;
using namespace sdsl;

typedef unsigned int u_int;

class CompactTrieIterator{
    private:
        bool at_end;
        bool at_root;
        u_int it;
        u_int parent_it;
        u_int pos_in_parent;
        //Louds representation to save tree structure
        bit_vector B;

        //Wavelet tree to save tree keys
        wt_int<> wt;

        //Chequear que efectivamente se necesitan todos o solo los de 0s
        rank_support_v<1> b_rank1; //ocupado
        rank_support_v<0> b_rank0; //ocupado
        select_support_mcl<0> b_sel0; //ocupado
        select_support_mcl<1> b_sel1; //ocupado

        /*
            recives index in bit vector
            returns index of next 0
        */
        u_int succ0(u_int it){
            u_int cant_0 = b_rank0(it);
            return b_sel0(cant_0 + 1);
        }
        
        /*
            recives index in bit vector
            returns index of previous 0
        */
        u_int prev0(u_int it){
            u_int cant_0 = b_rank0(it);
            return b_sel0(cant_0);
        }

        /*
            recives index of current node and the child that is required
            returns index of the nth child of current node
        */
        u_int child(u_int it, u_int n){
            return b_sel0(b_rank1(it+n)) + 1;
        }

        /*
            recives index of node whos children we want to count
            returns how many children said node has
        */
        u_int childrenCount(u_int it){
            return succ0(it) - it;
        }

        /*
            recives node index
            returns index of position in parent
        */
        u_int getPosInParent(u_int it){
            return b_sel1(b_rank0(it));
        }

        /*
            recives index of node
            return which child of its parent it is
        */
        u_int childRank(u_int it){
            u_int pos = getPosInParent(it);
            return pos - prev0(pos);
        }

        /*
            recives index of node
            returns index of parent node
        */  
        u_int parent(u_int it){
            u_int pos = getPosInParent(it);
            return prev0(pos) + 1;
        }

    public:

        CompactTrieIterator(bit_vector b, string s){
            B = b;
            it = 2;
            construct_im(wt, s, 'd');
            at_root = true;
            at_end = false;
            util::init_support(b_rank1,&B);
            util::init_support(b_rank0,&B);
            util::init_support(b_sel1,&B);
            util::init_support(b_sel0,&B);
        }

        u_int key(){
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

        bool atEnd(){
            return at_end;
        }

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

        void next(){
            if(at_root){
                throw "At root, doesn't have next";
            }
            if(at_end){
                throw "Iterator is atEnd";
            }

            u_int parent_child_count = childrenCount(parent_it);
            if(parent_child_count == pos_in_parent){
                at_end = true;
            }
            else{
                pos_in_parent++;
                it = child(parent_it, pos_in_parent);
            }
        }

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

        void seek(u_int seek_key){
            if(at_root){
                throw "At root, cant seek";
            }
            if(at_end){
                throw "At end, cant seek";
            }

            u_int parent_child_count = childrenCount(parent_it);

            u_int i = b_rank0(it)-2;
            u_int f = b_rank0(child(parent_it, parent_child_count))-2;
            
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
};

#endif