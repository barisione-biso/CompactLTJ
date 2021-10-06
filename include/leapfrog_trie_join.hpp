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
        bool at_end = false;
        uint64_t p = 0;
        uint64_t xp,x;
        uint64_t k;
        uint64_t key;
        u_int64_t depth;
        void chooseIterators(){
            /*
                debe elegir que iterador de cada indice ocupar
                de momento será el primero
            */
            for(auto ind: indexes){
                iterators.push_back(ind->getIterator(0));
            }
        }

        /*
            Return module a%b, supports negative numbers
        */
        uint64_t modulo(int a, uint64_t b){
            return (b + (a%b)) % b;
        }

    public:
        LTJ(vector<Index*> ind){
            indexes = ind;
            chooseIterators();
            k = iterators.size();
            depth = 0;
        }

        /*
            Prepares the iterators and finds first result
        */
        void leapfrog_init(){
            // cout<<"leapfrog_init"<<endl;
            for(auto it:iterators){
                if(it->atEnd()) at_end = true;
            }
            if(!at_end){
                sort(iterators.begin(), iterators.end());
                leapfrog_search();
            }   
        }

        /*
            Finds joins result for a variable if there are any
        */
        void leapfrog_search(){
            // cout<<"leapfrog_search"<<endl;
            xp = iterators[modulo(int(p)-1,k)]->key();
            while(true){
                x = iterators[p]->key();
                if(x==xp){
                    key = x;
                    return;
                }
                else{
                    iterators[p]->seek(xp);
                    if(iterators[p]->atEnd()){
                        at_end = true;
                        return;
                    }
                    else{
                        xp = iterators[p]->key();
                        p = modulo(p+1,k);
                    }
                }
            }
        }

        /*
            Moves all iterators to the next instance of the variable their are standing in
        */
        void leapfrog_next(){
            // cout<<"leapfrog_next"<<endl;
            iterators[p]->next();
            if(iterators[p]->atEnd()) at_end = true;
            else{
                p = modulo(p+1,k);
                leapfrog_search();
            }
        }

        /*
            Makes all iterators open to go to the next variable
        */
        void triejoin_open(){
            // cout<<"trijoin_open"<<endl;
            depth++;
            for(auto it:iterators){
                it->open();
            }
            leapfrog_init();
        }

        /*
            Makes all iterators go up to the previous variable
        */
        void triejoin_up(){
            for(auto it:iterators){
                it->up();
            }
            at_end = false;
            depth--;
        }

        /*
            Finds the first element of all iterators that is greater that seekKey
        */
        void leapfrog_seek(uint64_t seekKey){
            iterators[p]->seek(seekKey);
            if(iterators[p]->atEnd()) at_end = true;
            else{
                p = modulo(p+1,k);
                leapfrog_search();
            }
        }

        /*
            Implements the triejoin algorithm finding the join results
        */
        void triejoin(){
            vector<u_int64_t> v(3);
            u_int64_t i = 0;
            triejoin_open();

            while(true){
                if(!at_end){ 
                    v[i] = key;
                    if(depth < 3){
                        i++;
                        triejoin_open();
                    }
                    else{
                        cout<<"Ans: ";
                        for(int j=0; j<v.size(); j++){
                            cout<<v[j]<<" ";
                        }
                        cout<<endl;
                        leapfrog_next();
                    }
                }
                else{
                    if(depth==1)break;
                    i--;
                    triejoin_up();
                    leapfrog_next();  
                }
            }
        }

        //Probando

        void keys(){
            for(auto it:iterators){
                cout<<it->key()<<endl;
            }
        }

        u_int64_t get_key(){
            return key;
        }

    

    







};

#endif