#ifndef LEAPFROG_TRIE_JOIN_H
#define LEAPFROG_TRIE_JOIN_H

#include <iostream>
#include <vector>
#include <algorithm>

#include "index.hpp"
#include "iterator.hpp"


using namespace std;

class LTJ{
    
    private:
        vector<Iterator*> iterators;
        vector<Index*> indexes;
        void chooseIterators(){
            /*
                debe elegir que iterador de cada indice ocupar
                de momento será el primero
            */
            for(auto ind: indexes){
                iterators.push_back(ind->getIterator(0));
            }
        }

    public:
        LTJ(vector<Index*> ind){
            indexes = ind;
            chooseIterators();
        }
    

    // bool at_end = false;
    // uint64_t p = 0;
    // uint64_t xp,x;
    // uint64_t k = iterators.size();
    // uint64_t key;

    // void leapfrog_init(){
    //     for(auto it:iterators){s
    //         if(it.atEnd()) at_end = true;
    //     }
    //     if(!at_end){
    //         sort(iterators.begin(), iterators.end());
    //         leapfrog_search();
    //     }   
    // }

//     void leapfrog_search(){
//         xp = iterators[(p-1) % k].key();
//         while(true){
//             x = iterators[p].key();
//             if(x==xp){
//                 key = x;
//                 return;
//             }
//             else{
//                 iterators[p].seek(xp);
//                 if(iterators[p].atEnd()){
//                     at_end = true;
//                     return;
//                 }
//                 else{
//                     xp = iter[p].key();
//                     p = (p + 1) % k; 
//                 }
//             }
//         }
//     }

//     void leapfrog_next(){
//         iterators[p].next();
//         if(iter[p].atEnd()) at_end = true;
//         else{
//             p = (p+1)%k;
//             leapfrog_search();
//         }
//     }

//     void leapfrog_seek(uint64_t seekKey){
//         iterators[p].seek(seekKey);
//         if(iterators[p].atEnd()) at_end = true;
//         else{
//             p = (p+1)%k;
//             leapfrog_search();
//         }
//     }

};

#endif