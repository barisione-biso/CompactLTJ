#ifndef LEAPFROG_TRIE_JOIN_H
#define LEAPFROG_TRIE_JOIN_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>

#include "index.hpp"
#include "iterator.hpp"

using namespace std;

class LTJ{
    
    private:
        //BORRAR
        bool debug = false;
        //HASTA AQUI
        vector<Iterator*> iterators;
        vector<Index*> indexes;
        vector<Tuple*> query;
        map<string, set<uint64_t>> instances; 
        map<uint64_t, set<string>> variables_per_depth;
        map<string, uint64_t> variable_index_mapping;
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

        /*
            Resets all indexes to their start configuration
        */
        void resetIndexes(){
            at_end = false;
            for(auto i: indexes){
                i->resetIterators();
            }
        }

    public:
        LTJ(vector<Index*> &ind, vector<Tuple*> &q, map<string, uint64_t> &variables_to_index){
            indexes = ind;
            query = q;
            /*De momento se asume que todas las tablas tienen la misma dimensión*/
            dim = indexes[0]->getDim();
            chooseIterators();
            k = iterators.size();
            depth = 0;
            variable_index_mapping = variables_to_index;
            resetIndexes();
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
            if(debug)cout<<"xp es "<<xp<<endl;
            while(true){
                x = iterators[p]->key();
                if(debug)cout<<"x es "<<x<<endl;
                if(x==xp){
                    key = x;
                    if(debug)cout<<"se encontró key "<<key<<endl;
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
            if(debug)cout<<"Iterators en p es "<<iterators[p]->key()<<endl;
            if(debug)cout<<"La seek key es "<<seekKey<<endl;
            iterators[p]->seek(seekKey);
            if(debug)cout<<"Iterators en p paso a ser "<<iterators[p]->key()<<endl;
            if(debug)iterators[p]->atEnd() ? cout<<"Esta en end"<<endl : cout<<"No esta en end"<<endl;
            if(iterators[p]->atEnd()) at_end = true;
            else{
                p = modulo(p+1,k);
                if(debug)cout<<"Se llama a leapfrog_search"<<endl;
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

        void printAnswer(vector<u_int64_t> &v, Tuple* tuple, map<string, set<uint64_t>> &instances){
            cout<<"ANS: ";
            for(int j=0; j<v.size(); j++){
                if(tuple->get_term(j)->isVariable()){
                    instances[tuple->get_term(j)->getVariable()].insert(v[j]);
                }
                cout<<v[j]<<" ";
            }
            cout<<endl;
        }
        
        void goUpAndSearch(bool &is_variable, map<string, uint64_t> &current_depth, map<string, 
            uint64_t> &current_values, string &variable, Tuple* tupla){
            if(is_variable && current_depth[variable]==depth)current_values.erase(variable);
            triejoin_up();
            is_variable = tupla->get_term(depth-1)->isVariable();
            if(is_variable) {variable = tupla->get_term(depth-1)->getVariable();
            if(debug)cout<<"(3)At depth "<<depth<<" variable "<<variable<<endl;
            variables_per_depth[depth].insert(variable);}
            if(is_variable && current_depth[variable]==depth)current_values.erase(variable);        
            leapfrog_next();  
        }

        void triejoin(){
            Tuple* tupla;
            for(int j=0; j<query.size(); j++){
                if(debug)cout<<"at end es "<<at_end<<endl; 
                Tuple* tupla = query[j];
                //Se printea tupla
                tupla->printTuple();
                vector<uint64_t> v(dim);
                map<string, uint64_t> current_values;
                map<string, uint64_t> current_depth;
                map<uint64_t, set<uint64_t>::iterator> var_instances_it;
                u_int64_t i = 0;
                bool is_variable;
                string variable;
                // Se hace open de la raiz
                if(debug)cout<<"Se hace open de la raiz"<<endl;
                triejoin_open();
                //Se revisa si el término de la consulta es variable
                
                is_variable = tupla->get_term(depth-1)->isVariable();
                if(debug)is_variable ? cout<<"El termino es variable"<<endl : cout<<"El termino no es variable"<<endl;
                
                if(is_variable){
                    variable = tupla->get_term(depth-1)->getVariable();
                    cout<<"(1)At depth "<<depth<<" variable "<<variable<<endl;
                    variables_per_depth[depth].insert(variable);
                    //Chequear si la variable ya tiene instancias asociadas y hacer seek de dichas instancias.
                    if(instances[variable].size()!=0){
                        if(debug)cout<<"La variable ya tiene instancias asociadas, se hace seek"<<endl;
                        var_instances_it[depth] = instances[variable].begin();
                        if(debug)cout<<"La primera instancia encontrada "<<*var_instances_it[depth]<<endl;
                        if(debug)cout<<"pre_seek at end es "<<at_end<<endl;
                        leapfrog_seek(*var_instances_it[depth]);
                        if(debug)cout<<"at end es "<<at_end<<endl;
                    }
                    else{
                        leapfrog_search();
                    }
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
                        else if(is_variable && current_values.find(variable)!=current_values.end() && key!=current_values[variable]){
                            if(depth==1)break;
                            i--;
                            goUpAndSearch(is_variable, current_depth, current_values, variable, tupla);
                        }
                        else if(depth < dim){
                            // Si la variable actual no tenía un valor asociado 
                            // (para cuando se repite una variable por la query)
                            if(current_values.find(variable)==current_values.end()){
                                // Se guarda en current values el par variable key encontrado
                                current_values[variable] = v[i];
                                current_depth[variable] = depth;
                            }
                            i++;
                            triejoin_open();
                            is_variable = tupla->get_term(depth-1)->isVariable();
                            if(is_variable){
                                variable = tupla->get_term(depth-1)->getVariable();
                                cout<<"(2)At depth "<<depth<<" variable "<<variable<<endl;
                                variables_per_depth[depth].insert(variable);
                                if(current_values.find(variable)!=current_values.end()){
                                    leapfrog_seek(current_values[variable]);
                                }
                                else leapfrog_search();
                            }
                            else{
                                leapfrog_seek(tupla->get_term(depth-1)->getConstant());
                            }
                        }
                        else{
                            printAnswer(v, tupla, instances);
                            if(is_variable){
                                leapfrog_next();
                            }
                            else{
                                if(depth==1)break;
                                i--;
                                goUpAndSearch(is_variable, current_depth, current_values, variable, tupla);
                            }
                        }
                    }
                    else{
                        cout<<"Iterador at end"<<endl;
                        if(depth==1)break;
                        i--;
                        goUpAndSearch(is_variable, current_depth, current_values, variable, tupla); 
                    }
                }
                resetIndexes();
                depth = 0;
            }
        }

        void evaluate(){
            cout<<"In evaluate"<<endl;
            for(auto p: variables_per_depth){
                cout<<p.first<<": ";
                for(auto v: p.second){
                    cout<<v<<" ";
                }
                cout<<endl;
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