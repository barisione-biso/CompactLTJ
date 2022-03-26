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
    public:
    // private:
        vector<Iterator*> iterators;
        bool at_end;
        // Iterador con la key mas pequeña
        uint32_t p;
        uint32_t xp,x;
        // Cantidad de iteradores (Cuantas queries)
        uint32_t k;
        uint32_t key;
        uint32_t dim;
        bool debug=true;
        
        LeapfrogJoin(vector<Iterator*> its, uint32_t d, string &var){
            this->iterators = its;
            this->at_end = false;
            this->k = iterators.size();
            this->dim = d;
            this->p = 0;
            // depth = 0;
            // traverse();
        }

        LeapfrogJoin(){ }

        void clear(){
            iterators.clear();
        }

        /*
            Return module a%b, supports negative numbers
        */
        uint32_t modulo(int a, uint32_t b){
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

    // public:

        
        
        /*
            Returns true if any iterator of the LeapfrogJoin is at the end
        */
        bool is_at_end(){
            return at_end;
        }
        
        /*
            Returns the key on which all iterators are standing 
        */
        uint32_t get_key(){
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
                
            }   
        }

        void leapfrog_search(){
            cout<<"checking depths"<<endl;
            for(auto it: iterators){
                cout<<it->get_depth()<<endl;
            }
            // leapfrog_init();
            //TODO: averiguar si ese int(p) puede causar problemas con número más grandes, hasta donde debería llegar?
            if(debug){cout<<"Entrando a leapfrog_search"<<endl;}
            cout<<"p: "<<p<<endl;
            xp = iterators[modulo(int(p)-1,k)]->key();
            cout<<"xp: "<<xp<<endl;
            if(debug){cout<<"xp es "<<xp<<endl;}
            while(true){
                if(debug){cout<<"p es "<<p<<endl;}
                // cout<<"obteniendo x en search"<<endl;
                x = iterators[p]->key();
                cout<<"x: "<<x<<endl;
                // cout<<"obtuve x en search"<<endl;
                if(debug){cout<<"x es "<<x<<endl;}
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
        void leapfrog_seek(uint32_t seekKey){
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
                if(debug){cout<<"up indicator es "<<up_indicator[i]<<endl;}
                if(up_indicator[i]){
                    if(debug){cout<<"solo subi"<<endl;}
                    it->up();
                }
                else{
                    if(debug){cout<<"subi y bajo"<<endl;}
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

        void check_depths(map<uint32_t,uint32_t> &goal_depths){
            if(debug){cout<<"se hizo check_depths"<<endl;}
            cout<<"se hizo check_depths"<<endl;
            for(auto it: iterators){
                uint32_t index_tuple = it->getTuple();
                while(it->get_depth()>goal_depths[index_tuple]){
                    if(debug){cout<<"el depth "<<it->get_depth()<<" "<<goal_depths[index_tuple]<<endl;}
                    it->up();
                }
                if(debug){cout<<"la tupla "<<index_tuple<<" subio al nivel "<<it->get_depth();}
                cout<<"la tupla "<<index_tuple<<" subio al nivel "<<it->get_depth()<<endl;
            }
        }
};


class LTJ{
    public:
    // private:
        //BORRAR
        bool debug = true;
        //HASTA AQUI
        vector<Iterator*> iterators;
        vector<Index*> *indexes;
        vector<Tuple> *query;
        vector<Tuple*> modified_query;
        // int tuple_index = 0;
        map<string, set<uint32_t>> *variable_tuple_mapping;
        map<string, LeapfrogJoin> variable_lj_mapping;
        bool at_end;
        uint32_t key;
        uint32_t depth;
        uint32_t dim;
        vector<string> *gao;
        uint32_t limit;

        // Cosas para triejoin_tentativo
        bool show_results=false;
        map<string, int> gao_map;

        void clear(){
            gao_map.clear();
            for(auto p: variable_lj_mapping){
                p.second.clear();
            }
            variable_lj_mapping.clear();
            modified_query.clear();
            iterators.clear();
        }
        /*
            Return module a%b, supports negative numbers
        */
        uint32_t modulo(int a, uint32_t b){
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
            Creates one leapfrog join for each variable on the join and assignes it the iterators 
            asociated with the tuples that contain that variable
        */
        void createLeapfrogJoins(){
            for(auto it=variable_tuple_mapping->begin(); it!=variable_tuple_mapping->end(); it++){
                auto &p = *it;
            // for(auto p: variable_tuple_mapping){
                string var = p.first;
                vector<Iterator*> iter;
                for(auto tup: p.second){
                    iter.push_back(iterators[tup]);
                } 
                variable_lj_mapping[var] = LeapfrogJoin(iter, dim, var);
                // variable_lj_mapping.insert(make_pair(var, LeapfrogJoin(iter, dim, var)));
                // variable_lj_mapping[var] = new LeapfrogJoin(iter, dim, var);
                variable_lj_mapping[var].leapfrog_init();
            }
        }

        /*
            Returns a string containing the constants that are in the tuple, 
            it also adds the term asociated to the constant to the terms vector
        */
        string getConstantsOrder(Tuple &tuple, int &added_items, vector<Term> &terms){
            stringstream order;
            for(int i=0; i<dim; i++){
                Term* term = tuple.get_term(i);
                if(!term->isVariable()){
                    order<<i<<" ";
                    added_items++;
                    terms.push_back(*term);
                }
            }
            return order.str();
        }

        /*
            Returns a strign containing the variables that are in tuple, but ordered according to the gao 
            vector, it also adds the terms associated with each variable to the terms vector in the gao order
        */
        string getVariableOrder(Tuple &tuple, int &added_items, vector<Term> &terms){
            stringstream order;

            for(auto it=gao->begin(); it!=gao->end(); it++){
                string &var = *it;
            // for(auto var: gao){
                for(int i=0; i<dim; i++){
                    Term* term = tuple.get_term(i);
                    if(term->isVariable() && term->getVariable() == var){
                        order<<i;
                        added_items++;
                        if(added_items < dim)order<<" ";
                        terms.push_back(*term);
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
        void setIterators(){
            vector<string> required_orders;
            
            // for(auto tuple: &query)
            for(auto it=query->begin(); it!=query->end(); it++){
                Tuple &tuple = *it;
                stringstream order;
                int added_items = 0;
                vector<Term> terms;
                order<<getConstantsOrder(tuple, added_items, terms);
                order<<getVariableOrder(tuple, added_items, terms);
                order.seekp(-1, std::ios_base::end);
                required_orders.push_back(order.str());
                modified_query.push_back(new Tuple(terms));
            }  
            uint32_t index_tuple = 0;

            for(auto order : required_orders){
                iterators.push_back(new CurrentIterator(indexes->at(0)->getTrie(order), index_tuple));
                index_tuple++;
            }

            if(debug){
                cout<<"ORDERS: "<<endl;
                for(auto order : required_orders){
                    cout<<"-"<<order<<"-"<<endl;
                }
            }
        }

        void setGaoMap(){
            for(int i=0; i<gao->size(); i++){
                gao_map[gao->at(i)] = i;
            }
        }

        // void checkIterators(){
        //     cout<<"checking iterators"<<endl;

        //     for(auto it: iterators){
        //         cout<<"Checking it"<<endl;
        //         it->open();
        //         while(!it->atEnd()){
        //             cout<<it->key()<<" ";
        //             it->next();
        //         }
        //         cout<<endl;
        //         it->up();
        //     }
        // }

    // public:
        LTJ(vector<Index*> *ind, vector<Tuple> *q, vector<string> *gao_vector, map<string, set<uint32_t>> *variables_to_index, uint32_t lmt){
            this->indexes = ind;
            this->query = q;
            this->gao = gao_vector;
            dim = indexes->at(0)->getDim();
            setIterators();
            this->variable_tuple_mapping = variables_to_index;
            createLeapfrogJoins();
            depth = 0;
            limit = lmt;
            setGaoMap();
            at_end = false;
        }

        ~LTJ(){
            // for(auto var_lj : variable_lj_mapping){
            //     delete var_lj.second;
            // }

            for(auto tuple: modified_query){
                delete tuple;
            }

            for(auto it: iterators){
                delete it;
            }
        }

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
            Makes all iterators open to go to the next variable
        */
        void triejoin_open(){
            // cout<<"trijoin_open"<<endl;
            depth++;
            for(auto it:iterators){
                it->open();
            }
        }
  
        /*
            Returns the index of the variable in the given tuple. -1 if the variable isn't in the tuple
        */
        int get_var_index_in_tuple(Tuple *tuple, string &var){
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
        int get_gao_score(string &var){
            for(int i=0; i<gao->size(); i++){
                if(gao->at(i)==var)return i;
            }
            return -1;
        }
        
        /*
            Returns a vector indicating if each of the iterators associated with the variable should, 
            or can go up a level
        */
        void check_for_prev_value(string &var, int &gao_score, vector<bool> &should_go_up){
            if(debug){cout<<"cheching for prev value "<<var<<" "<<gao_score<<endl;}
            // vector<bool> should_go_up;
            for(auto tuple_index : variable_tuple_mapping->at(var)){
                Tuple *tuple = modified_query[tuple_index];
                int term_index = get_var_index_in_tuple(tuple, var);
                if(debug){cout<<"term_index es "<<term_index<<endl;}
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
                        else{
                            should_go_up.push_back(true);
                        }
                    }
                }
            }
            // return should_go_up;
        }
        
        void check_iterators_position(LeapfrogJoin* lj, string &var){
            if(debug){cout<<"checking iterators positions"<<endl;}
            //para cada iterador de lj obtener la posición en la query de la variable que se busca
            // y verificar que la altura en la que está calza con la altura que necesita
            map<uint32_t, uint32_t> goal_depths;
            // vector<int> goal_depths;
            for(auto tuple_index : variable_tuple_mapping->at(var)){
                Tuple *tuple = modified_query[tuple_index];
                goal_depths[tuple_index] = get_var_index_in_tuple(tuple, var);
                // goal_depths.push_back(get_var_index_in_tuple(tuple, var));
                if(debug){cout<<"la tupla "<<tuple_index<<" debe ir a "<<goal_depths[tuple_index]<<endl;}
            }
            if(debug){cout<<"goal depths tiene tamaño "<<goal_depths.size()<<endl;}
            lj->check_depths(goal_depths);
        }
        /*
            Goes up on all necessary iterators until gao_index and gao_score are the same. 
            If at the final position there is an iterator at_end, then it calls goUp.
        */
        bool goUpUntil(int gao_score, int &gao_index){
            int beg = gao_index;
            for(int i=beg; i>gao_score; i--){
                string var = gao->at(i);
                if(debug){cout<<"Going up on var "<<var<<endl;}
                LeapfrogJoin* lj = &variable_lj_mapping[var];
                vector<bool> should_go_up;
                // vector<bool> should_go_up = check_for_prev_value(var, gao_score);
                check_for_prev_value(var, gao_score,should_go_up);
                if(debug){cout<<"cheching should go up"<<endl;}
                if(debug){
                    for(auto v: should_go_up){
                        cout<<v<<" ";
                    }
                    cout<<endl;
                }
                if(debug){cout<<"en el up de goUpUntil "<<endl;}
                lj->up(should_go_up);
                // lj->up();
                gao_index--;
            }

            cout<<"depths after going up for"<<endl;
            for(auto it: iterators){
                cout<<it->get_depth()<<" ";
            }
            cout<<endl;
            // if(debug){
            //     cout<<"Iterators positions and keys:"<<endl;
            //     for(auto it: iterators){
            //         cout<<"depth: "<<it->get_depth()<<"/ key: "<<it->key()<<endl;
            //     }
            // }
            if(debug){cout<<"gao score is "<<gao_score<<" "<<gao->at(gao_score)<<endl;}
            LeapfrogJoin* lj = &variable_lj_mapping[gao->at(gao_score)];
            if(debug && lj->is_at_end()){
                cout<<"el iterador ya estaba en at end"<<endl;
            }
            check_iterators_position(lj, gao->at(gao_score));
            if(debug)cout<<"se hace next para "<<gao->at(gao_score)<<endl;
            lj->leapfrog_next();
            if(lj->is_at_end()){
                if(debug){cout<<"el iterador esta at en en goUpUntil"<<endl;}
                if(gao_score==0){
                    if(debug){cout<<"Cant go up"<<endl;}
                    return true;
                }
                if(debug){cout<<"going up again on "<<gao->at(gao_index)<<" index "<<gao_index<<endl;}
                return goUp(gao_index);
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
        bool goUp(int &gao_index){
            if(gao_index==0){
                return true;
            }
            else{
                return goUpUntil(gao_index-1, gao_index);
            }
        }

        void triejoin_definitivo(int &number_of_results){
            //I: Para mostrar tabla de resultados
            vector<vector<int>> results;
            vector<int> result(gao->size());
            //F: Para mostrar tabla de resultados
            uint32_t count=0;
            if(debug){cout<<"Starting Triejoin"<<endl;}
            triejoin_open();
            bool finished = false;
            //Donde nos encontramos en cada iterador?
            if(debug){
                cout<<"Iterators positions and keys:"<<endl;
                for(auto it: iterators){
                    cout<<"depth: "<<it->get_depth()<<"/ key: "<<it->key()<<endl;
                }
            }

            //Resolvemos las constantes para todas las tuplas
            if(debug){cout<<"Working with constants"<<endl;}
            for(int i=0; i<modified_query.size(); i++){
                Tuple* tuple = modified_query[i];
                for(int j=0; j<dim; j++){
                    Term* term = tuple->get_term(j);
                    if(!term->isVariable()){
                        if(debug){cout<<"Term no es variable es "<<term->getConstant()<<endl;}
                        iterators[i]->seek(term->getConstant());
                        if(!iterators[i]->atEnd() && iterators[i]->key() == term->getConstant()){
                            if(debug){cout<<"Se encontró la constante "<<term->getConstant()<<endl;}
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
            if(debug){cout<<"Constants handled"<<endl;}

            // leapfrog_init();

            //Donde nos encontramos en cada iterador?
            if(debug){
                cout<<"Iterators positions and keys:"<<endl;
                for(auto it: iterators){
                    cout<<"depth: "<<it->get_depth()<<"/ key: "<<it->key()<<endl;
                }
            }

            // cout<<"Iterators positions and keys:"<<endl;
            // for(auto it: iterators){
            //     cout<<"depth: "<<it->get_depth()<<"/ key: "<<it->key()<<endl;
            // }


            //Resolvemos las variables en el órden en el que aparecen en gao
            int gao_index = 0;
            while(gao_index < gao->size() && !finished){
                cout<<"depths before search"<<endl;
                for(auto it: iterators){
                    cout<<it->get_depth()<<" ";
                }
                cout<<endl;
                string var = gao->at(gao_index);
                if(debug){cout<<"buscando para var "<<var<<endl;}
                LeapfrogJoin* lj = &variable_lj_mapping[var];
                if(debug){cout<<"Se encontró LJ para "<<var<<endl;}
                lj->leapfrog_search();
                if(debug){cout<<"Se hizo search"<<endl;}
                int current_level = gao_index;
                while(current_level == gao_index){
                    if(lj->is_at_end()){
                        if(gao_index == 0)return;
                        else {
                            if(debug){cout<<"going up"<<endl;}
                            finished = goUp(gao_index);
                        }
                    }
                    else{
                        cout<<"gao index es "<<gao_index<<endl;
                        result[gao_index] = lj->get_key();
                        if(debug){cout<<var<<": "<<lj->get_key()<<endl;}
                        if(gao_index == gao->size()-1){
                            if(show_results){results.push_back(result);}
                            count++;
                            if(count == limit){
                                finished = true;
                                break;
                            }
                            if(debug){cout<<"Fin del resultado!"<<endl;}
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
                for(auto it=gao->begin(); it!=gao->end(); it++){
                    string &var = *it;
                // for(auto var : gao){
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
        }

        void keys(){
            for(auto it:iterators){
                cout<<it->key()<<endl;
            }
        }

        uint32_t get_key(){
            return key;
        }
};

#endif