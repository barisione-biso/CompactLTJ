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
        vector<Tuple*> query;
        bool at_end = false;
        uint64_t p = 0;
        uint64_t xp,x;
        uint64_t k;
        uint64_t key;
        u_int64_t depth;
        u_int64_t dim;
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
        LTJ(vector<Index*> &ind, vector<Tuple*> &q){
            indexes = ind;
            query = q;
            /*De momento se asume que todas las tablas tienen la misma dimensión*/
            dim = indexes[0]->getDim();
            chooseIterators();
            k = iterators.size();
            depth = 0;
        }

        /*
            Prepares the iterators and finds first result
        */
        // void leapfrog_init(){
        //     // cout<<"leapfrog_init"<<endl;
        //     for(auto it:iterators){
        //         if(it->atEnd()) at_end = true;
        //     }
        //     if(!at_end){
        //         sort(iterators.begin(), iterators.end());
        //         leapfrog_search();
        //     }   
        // }

        void leapfrog_init(){
            // cout<<"leapfrog_init"<<endl;
            for(auto it:iterators){
                if(it->atEnd()) at_end = true;
            }
            if(!at_end){
                sort(iterators.begin(), iterators.end());
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
            Finds the first element of all iterators that is greater or equal that seekKey
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
        // void triejoin(){
        //     //Solo consideramos primer tuple
        //     Tuple* tupla = query[0];
        //     vector<u_int64_t> v(dim);
        //     u_int64_t i = 0;
        //     if(tupla->get_term(depth)->isVariable()){
        //         cout<<"es variable"<<endl;
        //     }
        //     else{
        //         cout<<"es cte"<<endl;
        //     }
        //     triejoin_open();
        //     leapfrog_search();

        //     while(true){
        //         if(!at_end){ 
        //             v[i] = key;
        //             if(depth < dim){
        //                 i++;
        //                 triejoin_open();
        //                 leapfrog_search();
        //             }
        //             else{
        //                 cout<<"Ans: ";
        //                 for(int j=0; j<v.size(); j++){
        //                     cout<<v[j]<<" ";
        //                 }
        //                 cout<<endl;
        //                 leapfrog_next();
        //             }
        //         }
        //         else{
        //             if(depth==1)break;
        //             i--;
        //             triejoin_up();
        //             leapfrog_next();  
        //         }
        //     }
        // }

        void triejoin(){
            //Solo consideramos primer tuple
            Tuple* tupla = query[0];
            vector<u_int64_t> v(dim);
            u_int64_t i = 0;
            bool is_variable;
            triejoin_open();
            is_variable = tupla->get_term(depth-1)->isVariable();

            if(is_variable){
                leapfrog_search();
            }
            else{
                leapfrog_seek(tupla->get_term(depth-1)->getConstant());
            }

            while(true){
                
                if(!at_end){ 
                    v[i] = key;
                    if(!is_variable && key!=tupla->get_term(depth-1)->getConstant()){
                        leapfrog_next();  
                    }
                    else if(depth < dim){
                        i++;
                        triejoin_open();
                        is_variable = tupla->get_term(depth-1)->isVariable();
                        leapfrog_search();
                    }
                    else{
                        cout<<"ANS: ";
                        for(int j=0; j<v.size(); j++){
                            cout<<v[j]<<" ";
                        }
                        cout<<endl;
                        if(is_variable){
                            leapfrog_next();
                        }
                        else{
                            if(depth==1)break;
                            triejoin_up();
                            is_variable = tupla->get_term(depth-1)->isVariable();
                            leapfrog_next();
                        }
                    }
                }
                else{
                    if(depth==1)break;
                    i--;
                    triejoin_up();
                    is_variable = tupla->get_term(depth-1)->isVariable();
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