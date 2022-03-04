#ifndef COMPACT_TRIE_ITERATOR_H
#define COMPACT_TRIE_ITERATOR_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sdsl/vectors.hpp>
#include <sdsl/wavelet_trees.hpp>
#include <sdsl/wm_int.hpp>
#include "iterator.hpp"
#include "utils.hpp"
#include "compact_trie.hpp"

using namespace std;
using namespace sdsl;

class CompactTrieIterator: public Iterator{
    private:
        bool at_end;
        bool at_root;
        int depth;
        uint64_t it;
        uint64_t parent_it;
        uint64_t pos_in_parent;
        CompactTrie * compactTrie;

    public:

        /*
        Constructor from CompactTrie
        */
        CompactTrieIterator(CompactTrie* ct){
            compactTrie = ct;
            it = 2;
            at_root = true;
            at_end = false;
            depth = -1;
        }

        /*
            Destructor
        */
        ~CompactTrieIterator(){};

        int get_depth(){
            return depth;
        }
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
                return compactTrie->key_at(it);
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
                bool has_children = compactTrie->childrenCount(it) != 0;
                if(has_children){
                    parent_it = it;
                    it = compactTrie->child(it, 1);
                    pos_in_parent = 1;
                    depth++;
                }
                else throw "Node has no children";
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

            uint64_t parent_child_count = compactTrie->childrenCount(parent_it);
            if(parent_child_count == pos_in_parent){
                at_end = true;
            }
            else{
                pos_in_parent++;
                it = compactTrie->child(parent_it, pos_in_parent);
            }
        }
        
        /*
            Moves the iterator to the parent of the current node
        */
        void up(){
            if(at_root){
                throw "At root, cant go up";
            }
            depth--;
            it = parent_it;
            at_end = false;
            if(it==2){
                at_root = true;
            }
            else{
                pos_in_parent = compactTrie->childRank(it);
                parent_it = compactTrie->parent(it);
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

            // Nos indica cuantos hijos tiene el padre de el it actual ->O(1)
            uint64_t parent_child_count = compactTrie->childrenCount(parent_it);
            // Nos indica cuantos 0s hay hasta it - 2, es decir la posición en el string de el char correspondiente a la posición del it -> O(1)
            uint64_t i = compactTrie->b_rank0(it)-2;
            // Nos indica la posición en el string de el char correspondiente a la posición del ultimo hijo del padre del it. -> O(1)
            uint64_t f = compactTrie->b_rank0(compactTrie->child(parent_it, parent_child_count))-2;
            
            bool found = false;

            // cout<<"RNV: "<<compactTrie->wt.range_next_value(seek_key, i, f)<<endl;
            for(i=i; i<=f; i++){

                if(compactTrie->get_wt_at(i)>=seek_key){
                    // cout<<"wt at: "<<compactTrie->get_wt_at(i)<<endl;
                    // cout<<"i RNV: "<<i<<endl;
                    it = compactTrie->b_sel0(i+2)+1;
                    pos_in_parent = compactTrie->childRank(it);
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
            compactTrie->storeToFile(file_name);
            // ofstream stream(file_name);
            // if(stream.is_open()){
            //     stream<<B.size()<<'\n';
            //     for(uint64_t i=0; i<B.size(); i++){
            //         stream<<B[i]<<" ";
            //     }
            //     stream<<'\n';
            //     for(uint64_t i=0; i<wt.size(); i++){
            //         stream<<wt[i]<<" ";
            //     }
            // }
            // stream.close();
        }

        /*
            Loads Compact Trie from file restoring B and the Wavelet Tree
        */
        // void loadFromFile(string file_name){
        //     ifstream stream(file_name);
        //     uint64_t B_size;
        //     string s;
        //     uint64_t val;
        //     at_root = true;
        //     at_end = false;
        //     it = 2;

        //     if(stream.is_open()){
        //         stream>>B_size;
        //         B = bit_vector(B_size);
        //         for(uint64_t i=0; i<B_size; i++){
        //             stream>>val;
        //             B[i] = val;
        //         }
        //         stream.ignore(numeric_limits<streamsize>::max(),'\n');
        //         getline(stream, s);
        //     }
        //     stream.close();
            
        //     construct_im(wt, s, 'd');
        //     initializeSupport();
        // }
        /*
            Returns the iterator to the start of the data
        */
        void backToStart(){
            //Confirmar que esto no causa problemas
            it = 2;
            at_end = false;
            at_root = true;
        }
        
        /*
            Returns pointer to CompactTrie from which the iterator is constructed 
        */
        CompactTrie* getCompactTrie(){
            return compactTrie;
        }

        // Temporal 
        void getIteratorPos(){
            cout<<"it esta en: "<<it<<endl;
        }
};

#endif