#ifndef LEAPFROG_TRIE_JOIN_H
#define LEAPFROG_TRIE_JOIN_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>

#include "index.hpp"
#include "iterator.hpp"
#include "config.hpp"

using namespace std;

class LeapfrogJoin{
    private:
        vector<Iterator*> iterators;
        bool at_end;
        // Iterador con la key mas pequeña
        uint64_t p;
        uint64_t xp,x;
        // Cantidad de iteradores (Cuantas queries)
        uint64_t k;
        uint64_t key;
        uint64_t dim;
        bool debug=false;
        

        /*
            Return module a%b, supports negative numbers
        */
        uint64_t modulo(int a, uint64_t b){
            return (b + (a%b)) % b;
        }

        void traverse(){
            int i=0;
            for(auto it: iterators){
                cout<<"Iterator "<<i<<endl;
                while(true){
                    string instr;
                    cin>>instr;

                    try{
                        if(instr=="open")it->open();
                        if(instr=="next")it->next();
                        if(instr=="end")cout<<it->atEnd()<<endl;
                        if(instr=="key")cout<<it->key()<<endl;
                        // if(instr=="show")ct.showValues();
                        if(instr=="up")it->up();
                        if(instr=="seek"){
                            int x;
                            cin>>x;
                            it->seek(x);
                        }
                        if(instr=="close")break;
                    }
                    catch(const char* msg){
                        cerr<<msg<<endl;
                    }
                }
                i++;
            }

            for(auto it: iterators){
                it->backToStart();
            }
        }

    public:

        LeapfrogJoin(vector<Iterator*> &its, uint64_t d, string var){
            iterators = its;
            at_end = false;
            k = iterators.size();
            dim = d;
            // depth = 0;
            // traverse();
        }
        
        /*
            Returns true if any iterator of the LeapfrogJoin is at the end
        */
        bool is_at_end(){
            return at_end;
        }
        
        /*
            Returns the key on which all iterators are standing 
        */
        uint64_t get_key(){
            return key;
        }
        
        /*
            Prepares the iterators and finds first result
        */
        void leapfrog_init(){
                for(auto it:iterators){
                    if(it->atEnd()) at_end = true;
                }
                if(!at_end){
                    sort(iterators.begin(), iterators.end());
                    p = 0;
                }   
        }

        void leapfrog_search(){
            //TODO: averiguar si ese int(p) puede causar problemas con número más grandes, hasta donde debería llegar?
            if(debug)cout<<"Entrando a leapfrog_search"<<endl;
            xp = iterators[modulo(int(p)-1,k)]->key();
            if(debug)cout<<"xp es "<<xp<<endl;
            while(true){
                if(debug)cout<<"p es "<<p<<endl;
                x = iterators[p]->key();
                if(debug)cout<<"x es "<<x<<endl;
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
            iterators[p]->next();
            if(iterators[p]->atEnd()) at_end = true;
            else{
                p = modulo(p+1,k);
                leapfrog_search();
            }
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
            Moves iterators whos index in up_indicator is true. Used when only some of the iterators
            associated with this variable should go up.
            It doesn't move values horizontaly in the new level, it only sets them up so that it 
            the next LeapfrogJoin can use them
        */
        void up(vector<bool> &up_indicator){
            for(int i=0; i<iterators.size(); i++){
                auto it = iterators[i];
                if(up_indicator[i]){
                    it->up();
                }
                else{
                    it->up();
                    it->open();
                }
            }
            at_end = false;
        }

        /*
            Moves all iterators related to LeapfrogJoin to the level above their current level 
            except those in the first level after the root. 
            It doesn't move values horizontaly in the new level, it only sets them up so that it 
            the next LeapfrogJoin can use them
        */
        void up(){
            for(auto it:iterators){
                if(it->get_depth() == 0){
                    it->up();
                    it->open();
                }
                else{
                    it->up();
                }
            }
            at_end = false;
        }

        /*
            Moves all iterators to the next level, provided they are not in the last level, 
            in that case it does nothing.
            It doesn't move values horizontaly in the new level, it only sets them up so that it 
            the next LeapfrogJoin can use them
        */
        void open(){
            for(auto it: iterators){
                if(it->get_depth()<dim-1){
                    it->open();
                }
            }
            at_end = false;
            //QUESTION: hay que hacer leapfrog init cada vez?
            leapfrog_init();
            // p = 0;
        }

        void check_depths(vector<int> goal_depths){
            int i=0;
            for(auto it: iterators){
                while(it->get_depth()>goal_depths[i]){
                    it->up();
                }
                i++;
            }
        }
};


class LTJ{
    
    private:
        //BORRAR
        bool debug = true;
        //HASTA AQUI
        vector<Iterator*> iterators;
        vector<Index*> indexes;
        vector<Tuple*> query;
        vector<Tuple*> modified_query;
        int tuple_index = 0;
        map<string, set<uint64_t>> instances; 
        map<uint64_t, set<string>> variables_per_depth;
        map<string, set<uint64_t>> variable_tuple_mapping;
        map<string, LeapfrogJoin*> variable_lj_mapping;
        bool at_end = false;
        uint64_t p = 0;
        uint64_t xp,x;
        uint64_t k;
        uint64_t key;
        uint64_t iterator_count;
        u_int64_t depth;
        u_int64_t dim;
        vector<string> gao;
        uint64_t limit;

        // Cosas para triejoin_tentativo
        vector<map<string, set<uint64_t>>> instances_per_query;
        bool show_results=true;
        map<string, int> gao_map;




        /*
            CURRENTLY: Chooses first order of the elements in the table
            TODO: Should choose and order for the table based on what the user whants  
        */  
        // void chooseOrder(){
        //     /*
        //         debe elegir que iterador de cada indice ocupar
        //         de momento será el primero
        //     */
        //     for(auto ind: indexes){
        //         iterators.push_back(ind->getIterator(0));
        //     }
        // }

        /*
            Return module a%b, supports negative numbers
        */
        uint64_t modulo(int a, uint64_t b){
            return (b + (a%b)) % b;
        }

        /*
            Resets all iterators to their start configuration
        */
        void resetIterators(){
            at_end = false;
            for(auto it: iterators){
                it->backToStart();
            }
            // for(auto i: indexes){
            //     i->resetIterators();
            // }
        }

        /* 
            Adds one iterator for each element in the query to the the iterators vector
        */
        // void addIterators(){
        //     CompactTrie *ct = iterators[0]->getCompactTrie();
        //     for(int i=1; i<query.size(); i++){
        //         iterators.push_back(new CompactTrieIterator(ct));
        //     }
        // }

        /*
            Creates one leapfrog join for each variable on the join and assignes it the iterators 
            asociated with the tuples that contain that variable
        */
        void createLeapfrogJoins(){
            for(auto p: variable_tuple_mapping){
                string var = p.first;
                vector<Iterator*> iter;
                for(auto tup: p.second){
                    iter.push_back(iterators[tup]);
                } 
                variable_lj_mapping[var] = new LeapfrogJoin(iter, dim, var);
                variable_lj_mapping[var]->leapfrog_init();
            }
        }

        /*
            Returns a string containing the constants that are in the tuple, 
            it also adds the term asociated to the constant to the terms vector
        */
        string getConstantsOrder(Tuple* tuple, int &added_items, vector<Term*> &terms){
            stringstream order;
            for(int i=0; i<dim; i++){
                Term* term = tuple->get_term(i);
                if(!term->isVariable()){
                    order<<i<<" ";
                    added_items++;
                    terms.push_back(term);
                }
            }
            return order.str();
        }

        /*
            Returns a strign containing the variables that are in tuple, but ordered according to the gao 
            vector, it also adds the terms associated with each variable to the terms vector in the gao order
        */
        string getVariableOrder(Tuple* tuple, vector<string> gao, int &added_items, vector<Term*> &terms){
            stringstream order;
            for(auto var: gao){
                for(int i=0; i<dim; i++){
                    Term* term = tuple->get_term(i);
                    if(term->isVariable() && term->getVariable() == var){
                        order<<i;
                        added_items++;
                        if(added_items < dim)order<<" ";
                        terms.push_back(term);
                        break;
                    }
                }
            }
            return order.str();
        }

        /*
            Adds iterators to the iteratos vector creating a CompactTrieIterator using the order 
            that is required by gao. It also stores in modified query, the updated version of the query that follows the gao order.
        */
        void setIterators(vector<string> &gao){
            vector<string> required_orders;
            for(auto tuple: query){
                stringstream order;
                int added_items = 0;
                vector<Term*> terms;
                order<<getConstantsOrder(tuple, added_items, terms);
                order<<getVariableOrder(tuple, gao, added_items, terms);
                order.seekp(-1, std::ios_base::end);
                required_orders.push_back(order.str());
                modified_query.push_back(new Tuple(terms));
            }

            for(auto order : required_orders){
                iterators.push_back(new CurrentIterator(indexes[0]->getTrie(order)));
            }

            if(debug){
                cout<<"ORDERS: "<<endl;
                for(auto order : required_orders){
                    cout<<"-"<<order<<"-"<<endl;
                }
            }
            // cout<<"ORDERS: "<<endl;
            // for(auto order : required_orders){
            //     cout<<"-"<<order<<"-"<<endl;
            // }
        }

        void setGaoMap(){
            for(int i=0; i<gao.size(); i++){
                gao_map[gao[i]] = i;
            }
        }

    public:
        LTJ(vector<Index*> &ind, vector<Tuple*> &q, vector<string> &gao_vector, map<string, set<uint64_t>> &variables_to_index, uint64_t lmt){
            // cout<<"Calling LTJ constructor"<<endl;
            // De moemento ind tiene sólo uno
            indexes = ind;
            query = q;
            gao = gao_vector;
            /*De momento se asume que todas las tablas tienen la misma dimensión*/
            dim = indexes[0]->getDim();
            
            setIterators(gao);
            // Elige el orden
            // chooseOrder();
            // Agrega todos los iteradores al vector de iteradores. Uno por cada tupla de la query
            // addIterators();
            variable_tuple_mapping = variables_to_index;
            createLeapfrogJoins();
            //k es la cantidad de iteradores que se recorrerán que para la nueva solución son la cantidad de 
            //tuplas en q
            // k = q.size();
            k = iterators.size();
            depth = 0;
            
            // resetIndexes();
            //para triejoin_tentativo
            instances_per_query.resize(query.size());
            limit = lmt;
            setGaoMap();
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
            //TODO: averiguar si ese int(p) puede causar problemas con número más grandes, hasta donde debería llegar?
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
            cout<<"leapfrog_next"<<endl;
            cout<<"p is "<<p<<endl;
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
            // cout<<"Opened"<<endl;
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
                        // Caso v1 v1
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
                resetIterators();
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
        
        /*
            Returns the index of the variable in the given tuple. -1 if the variable isn't in the tuple
        */
        int get_var_index_in_tuple(Tuple *tuple, string var){
            for(int i=0; i<dim; i++){
                Term *term = tuple->get_term(i);
                if(term->isVariable() && term->getVariable()==var){
                    return i;
                }
            }
            return -1;
        }

        /*
            Returns gao score for the given variable 
        */
        int get_gao_score(string var){
            for(int i=0; i<gao.size(); i++){
                if(gao[i]==var)return i;
            }
            return -1;
        }
        
        /*
            Returns a vector indicating if each of the iterators associated with the variable should, 
            or can go up a level
        */
        vector<bool> check_for_prev_value(string var, int gao_score){
            vector<bool> should_go_up;
            for(auto tuple_index : variable_tuple_mapping[var]){
                Tuple *tuple = modified_query[tuple_index];
                int term_index = get_var_index_in_tuple(tuple, var);
                if(term_index == 0){
                    should_go_up.push_back(false);
                }
                else{
                    Term *prev_term = tuple->get_term(term_index-1);
                    if(!prev_term->isVariable()){
                        should_go_up.push_back(false);
                    }
                    else{
                        //Si el termino previo es una variable con < gao score que el que buscamos
                        string prev_var = prev_term->getVariable();
                        if(get_gao_score(prev_var) < gao_score){
                            should_go_up.push_back(false);
                        }
                        should_go_up.push_back(true);
                    }
                }
            }
            return should_go_up;
        }
        
        void check_iterators_position(LeapfrogJoin* lj, string var){
            if(debug)cout<<"checking iterators positions"<<endl;
            //para cada iterador de lj obtener la posición en la query de la variable que se busca
            // y verificar que la altura en la que está calza con la altura que necesita
            vector<int> goal_depths;
            for(auto tuple_index : variable_tuple_mapping[var]){
                Tuple *tuple = modified_query[tuple_index];
                goal_depths.push_back(get_var_index_in_tuple(tuple, var));
            }
            lj->check_depths(goal_depths);
        }
        /*
            Goes up on all necessary iterators until gao_index and gao_score are the same. 
            If at the final position there is an iterator at_end, then it calls goUp.
        */
        bool goUpUntil(int gao_score, int &gao_index){
            int beg = gao_index;
            for(int i=beg; i>gao_score; i--){
                string var = gao[i];
                if(debug)cout<<"Going up on var "<<var<<endl;
                LeapfrogJoin* lj = variable_lj_mapping[var];
                vector<bool> should_go_up = check_for_prev_value(var, gao_score);
                if(debug)cout<<"cheching should go up"<<endl;
                if(debug){
                    for(auto v: should_go_up){
                        cout<<v<<" ";
                    }
                    cout<<endl;
                }

                lj->up(should_go_up);
                // lj->up();
                gao_index--;
            }
            if(debug)cout<<"Iterators positions and keys:"<<endl;
            for(auto it: iterators){
                if(debug)cout<<"depth: "<<it->get_depth()<<"/ key: "<<it->key()<<endl;
            }
            if(debug)cout<<"gao score is "<<gao_score<<" "<<gao[gao_score]<<endl;
            LeapfrogJoin* lj = variable_lj_mapping[gao[gao_score]];
            check_iterators_position(lj, gao[gao_score]);
            if(debug)cout<<"Iterators positions and keys:"<<endl;
            for(auto it: iterators){
                if(debug)cout<<"depth: "<<it->get_depth()<<"/ key: "<<it->key()<<endl;
            }
            lj->leapfrog_next();
            if(lj->is_at_end()){
                if(gao_score==0){
                    if(debug)cout<<"Cant go up"<<endl;
                    return true;
                }
                if(debug)cout<<"going up again on "<<gao[gao_index]<<" index "<<gao_index<<endl;
                return goUp(gao[gao_index], gao_index);
            }
            else{
                return false;
            }
        }
        
        /*
            Checks until what variable should all iterators go up, based on the previous variable with 
            the smalles gao_score. It moves the iterators up until they can search for a new match or 
            en up in the root
        */
        bool goUp(string var, int &gao_index){
            //Cual es la variable de menor gao a la que se llega
            //1. En que consultas está la variable actual?
            //2. En cada consulta cual era la variable anterior
            //3. Ver cual es la variable anterior con menor gao
            if(debug)cout<<"Index in each tuple of "<<var<<endl;
            vector<pair<int, string>> gao_scores;
            for(auto tuple_index : variable_tuple_mapping[var]){
                if(debug)cout<<"tuple_index "<<tuple_index<<endl;
                Tuple *tuple = modified_query[tuple_index];
                if(debug)cout<<"got tuple "<<tuple_index<<" successfuly"<<endl;
                int term_index = get_var_index_in_tuple(tuple, var);
                if(debug)cout<<"term_index "<<term_index<<endl;
                if(term_index == 0){
                    continue;
                }
                else{
                    Term *prev_term = tuple->get_term(term_index-1);
                    if(!prev_term->isVariable()){
                        
                        //TODO: Esto no se debería hacer porque altera los valores de at_end de los LJ
                        // iterators[tuple_index]->up();
                        // iterators[tuple_index]->open();
                        // //Hacer hacer up y luego open para quedar en el primer valor de esta misma variable
                    }
                    else{
                        int gao_score = get_gao_score(prev_term->getVariable());
                        if(debug)cout<<"gao score prev term "<<gao_score<<endl;
                        gao_scores.push_back(make_pair(gao_score, prev_term->getVariable()));
                    }
                }
            }
            sort(gao_scores.begin(), gao_scores.end());
            if(gao_scores.size() == 0){
                return true;
            }
            else{
                int new_gao_index = gao_scores[0].first;
                if(debug)cout<<"new gao index "<<new_gao_index<<endl;
                return goUpUntil(new_gao_index, gao_index);
            }


        }

        void triejoin_definitivo(int &number_of_results){
            //I: Para mostrar tabla de resultados
            vector<vector<int>> results;
            vector<int> result(gao.size());
            //F: Para mostrar tabla de resultados
            uint64_t count=0;
            if(debug)cout<<"Starting Triejoin"<<endl;
            triejoin_open();
            bool finished = false;
            //Donde nos encontramos en cada iterador?
            if(debug)cout<<"Iterators positions and keys:"<<endl;
            for(auto it: iterators){
                if(debug)cout<<"depth: "<<it->get_depth()<<"/ key: "<<it->key()<<endl;
            }

            //Resolvemos las constantes para todas las tuplas
            if(debug)cout<<"Working with constants"<<endl;
            for(int i=0; i<modified_query.size(); i++){
                Tuple* tuple = modified_query[i];
                for(int j=0; j<dim; j++){
                    Term* term = tuple->get_term(j);
                    if(!term->isVariable()){
                        if(debug)cout<<"Term no es variable es "<<term->getConstant()<<endl;
                        iterators[i]->seek(term->getConstant());
                        if(!iterators[i]->atEnd() && iterators[i]->key() == term->getConstant()){
                            if(debug)cout<<"Se encontró la constante "<<term->getConstant()<<endl;
                            iterators[i]->open();
                        }
                        else{
                            // Si es que el valor no es igual a la constante entonces no 
                            // hay valores que cumplan esta tupla
                            return;
                        }
                        // cout<<"se movió iterador "<<i<<" a "<<iterators[i]->key()<<endl;
                    }
                    else break;
                }
            }
            if(debug)cout<<"Constants handled"<<endl;

            //Donde nos encontramos en cada iterador?
            if(debug)cout<<"Iterators positions and keys:"<<endl;
            for(auto it: iterators){
                if(debug)cout<<"depth: "<<it->get_depth()<<"/ key: "<<it->key()<<endl;
            }


            //Resolvemos las variables en el órden en el que aparecen en gao
            int gao_index = 0;
            while(gao_index < gao.size() && !finished){
                string var = gao[gao_index];
                if(debug)cout<<"buscando para var "<<var<<endl;
                LeapfrogJoin* lj = variable_lj_mapping[var];
                if(debug)cout<<"Se encontró LJ para "<<var<<endl;
                
                // if(lj->is_at_end()){
                //     cout<<"variable was in end"<<endl;
                //     try{
                //         lj->up();
                //         lj->leapfrog_next();
                //         gao_index--;
                //     }
                //     catch(const char *msg){
                //         return;
                //     }
                // }
                // if(from_up){
                //     lj->leapfrog_next();
                //     from_up = false;
                // }
                // else{
                //     // cout<<"LJ not at end"<<endl;
                    
                //     // cout<<"Se hizo search en LJ"<<endl;
                // }
                lj->leapfrog_search();
                if(debug)cout<<"Se hizo search"<<endl;
                int current_level = gao_index;
                while(current_level == gao_index){
                    if(lj->is_at_end()){
                        if(gao_index == 0)return;
                        else {
                            try{
                                finished = goUp(var, gao_index);
                                // Si lj tiene sólo un iterador podemos llegar y subir
                                // lj->up();
                                // from_up = true;
                                // gao_index--;
                            }
                            catch(const char *msg){
                                return;
                            }
                            
                        }
                    }
                    else{
                        result[gao_index] = lj->get_key();
                        if(debug)cout<<var<<": "<<lj->get_key()<<endl;
                        if(gao_index == gao.size()-1){
                            if(show_results)results.push_back(result);
                            count++;
                            if(count == limit){
                                finished = true;
                                break;
                            }
                            if(debug)cout<<"Fin del resultado!"<<endl;
                            lj->leapfrog_next();
                        }
                        else{
                            lj->open();
                            gao_index++;
                            // cout<<"found result"<<endl;
                        }
                    }
                } 
            }

            number_of_results = count;
            if(show_results){
                for(auto var : gao){
                    cout<<var<<"|";
                }
                cout<<endl;

                for(auto res : results){
                    for(auto val: res){
                        cout<<val<<"  |";
                    }
                    cout<<endl;
                }
                cout<<endl;
            }
            
            // ofstream stream("../data/result.txt");
            // if(stream.is_open()){
            //     stream<<"Termino ejecución"<<'\n';
            //     stream<<"Cantidad de resultados "<<results.size()<<'\n';
            // }
            // stream.close();
        }
        /*
        void trijoin_tentativo(){
            // Aquí se irán guardando los resultados que se encuentren para cada tupla 
            vector<vector<uint64_t>> answers_per_tuple(vector<uint64_t>(query.size(), vector<uint64_t> (dim)));
            map<string, uint64_t> variable_mapping;
            triejoin_open();
            // Aquí se guardan los i-esimos terms de cada query, en un principio, el primer term de cada query
            vector<Term*> query_terms;

            while(true){
                // Guardamos en query_terms los valores de los terms al depth en el que estamos
                for(auto tuple : query){
                    query_terms.push_back(tuple->get_term(depth-1));
                }

                // Iteramos por query_terms usando tuple_id como identificador de a que tupla pertenece el temino que estamos mirando
                for(auto tuple_id=0; tuple_id<query_terms.size(); tuple_id++){
                    Term* term = query_terms[tuple_id];
                    is_variable = term->isVariable();
                    // Si es variable vamos a buscar el primer valor que esté en todas las tablas a este depth 
                    if(is_variable){
                        variable = term->getVariable();
                        
                        leapfrog_search();
                        //si estamos mas allá del primer término de la tupla verificar que dicha variable no tenga unvalor asignado ya en su misma tupla
                        //si ya tiene un valor asignado hacer seek
                        //si estamos mas allá del primer término de la tupla verificar que dicha variable no tenga un valor asignado en las otras tuplas

                    }
                    else{
                        leapfrog_seek(term->getConstant());
                    }

                    if(!at_end){
                        answers_per_tuple[tuple_id][depth-1] = key;
                        if(is_variable)variable_mapping[variable] = key;
                        if(depth < dim && is_variable){
                            instances_per_query[tuple_id][varible].insert(key)
                        }
                    }
                }
            }
            

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

            while(true){
                
            }
        }
        */

        
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