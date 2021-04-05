#ifndef COMPACT_TRIE_H
#define COMPACT_TRIE_H

#include <sdsl/vectors.hpp>
#include <vector>
#include <iostream>
#include <sdsl/bp_support.hpp>
#include <sdsl/wavelet_trees.hpp>

using namespace std;
using namespace sdsl;

typedef unsigned int u_int;


 
class CompactTrie{

    private:
        bool at_end;
        bool at_root;
        u_int it_nodes;
        u_int it_keys;

        bit_vector B;
        vector<u_int> S;
        rank_support_v<1> b_rank1; 
        rank_support_v<0> b_rank0;
        select_support_mcl<0> b_sel0;
        select_support_mcl<1> b_sel1;
        bp_support_gg<> bp;


        u_int accessS(int it){
            return S[b_rank1(it+1)-1];
        }

        u_int succ0(u_int it){
            u_int cant_0 = b_rank0(it);
            return b_sel0(cant_0 + 1);
        }

        u_int succ1(u_int it){
            u_int cant_1 = b_rank1(it);
            return b_sel1(cant_1 + 1);
        }

        u_int prev0(u_int it){
            u_int cant_0 = b_rank0(it);
            return b_sel0(cant_0);
        }

        u_int prev1(u_int it){
            u_int cant_1 = b_rank1(it);
            return b_sel1(cant_1);
        }

        //recives it_key and identifies which child it is
        u_int childId(u_int it){
            u_int prev_rank = b_rank1(prev0(it));
            u_int current_rank = b_rank1(it+1);
            return current_rank - prev_rank;
        }

        //gives position of the subtree asociated with the child number num
        u_int child(u_int it, u_int num){
            return bp.find_close(succ0(it) - num) + 1;
        }

        //returns amount of children of node identifies by it_keys
        u_int children(u_int it_key){
            return b_rank1(succ0(it_key)) - b_rank1(prev0(it_key));
        }

        //returns position of subtree asociated with parent of current node
        u_int parent(u_int it){
            return prev0(bp.find_open(prev0(it))) + 1;
        }

        //recives it_nodes and identifies which child it is
        u_int childRank(u_int it){
            u_int p = bp.find_open(prev0(it));
            return succ0(p) - p;
        }

        //For the wavelet tree
        wt_int<> wt;

    public:

        CompactTrie(bit_vector b, string s){
            B = b;
            // construct_im(wt, s, 'd');
            at_root = true;
            it_nodes = 3;
            it_keys = 1;
            at_end = false;
            util::init_support(b_rank1,&B);
            util::init_support(b_rank0,&B);
            util::init_support(b_sel1,&B);
            util::init_support(b_sel0,&B);
            util::init_support(bp, &B);
        }
    
        void open(){
            if(at_root){
                at_root = false;
            }

            it_keys = it_nodes;
            it_nodes = succ0(it_nodes) + 1;
            at_end = false;    
        }

        u_int key(){
            if(at_end){
                throw "Iterator is atEnd";
            }
            return accessS(it_keys);
        }

        void next(){
            //pending checking for at end
            u_int child_id = childId(it_keys);
            u_int cant_children = children(it_keys);

            if(child_id==cant_children){
                at_end = true;
            }
            else{
                it_keys++;
                child_id = childId(it_keys);
                it_nodes = child(it_keys, child_id);
            }
        }

        bool atEnd(){
            return at_end;
        }

        void showValues(){
            cout<<"it_nodes es "<<it_nodes<<endl;
            cout<<"it_key es "<<it_keys<<endl;
        }

        void up(){
            it_nodes = parent(it_nodes);
            u_int child_rank = childRank(it_nodes);
            u_int grandfather = parent(it_nodes);
            it_keys = b_sel1(b_rank1(grandfather) + child_rank);
            at_end = false;
        }

        void seek(u_int seek_key){
            u_int start_index = it_keys;
            u_int end_index = succ0(it_keys);
            bool found = false;

            for(u_int i=start_index; i<end_index; i++){
                if(accessS(i)>=seek_key){
                    it_keys = i;
                    u_int child_id = childId(it_keys);
                    it_nodes = child(it_keys, child_id);
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