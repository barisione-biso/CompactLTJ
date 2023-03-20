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

#include <unordered_map>
#include <unordered_set>

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
        uint32_t count; //NEW DIEGO
        bool debug=false;

        LeapfrogJoin(vector<Iterator*> its, uint32_t d, string &var){
            this->iterators = its;
            this->at_end = false;
            this->k = iterators.size();
            this->dim = d;
            this->p = 0;
            this->count = 0; //NEW DIEGO
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
            //return (b + (a%b)) % b;
	   if(a < 0){return b-1;}
	   else{return a%b;}
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
            // cout<<"checking depths"<<endl;
            // for(auto it: iterators){
            //     cout<<it->get_depth()<<endl;
            // }
            // leapfrog_init();
            //TODO: averiguar si ese int(p) puede causar problemas con número más grandes, hasta donde debería llegar?
            if(debug){cout<<"Entrando a leapfrog_search"<<endl;}
            // cout<<"p: "<<p<<endl;
            xp = iterators[modulo(int(p)-1,k)]->key();
            // cout<<"xp: "<<xp<<endl;
            if(debug){cout<<"xp es "<<xp<<endl;}
            while(true){
                if(debug){cout<<"p es "<<p<<endl;}
                // cout<<"obteniendo x en search"<<endl;
                x = iterators[p]->key();
                // cout<<"x: "<<x<<endl;
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

        void leapfrog_search_new(){
            xp = iterators[modulo(int(p)-1,k)]->key();
            while(true){
                iterators[p]->seek(xp);  //NEW DIEGO
                if(iterators[p]->atEnd()){   //NEW DIEGO
                        at_end = true;
                        return;
                }
                x = iterators[p]->key();
                if(debug){cout<<"x es "<<x<<endl;}
                if(x==xp){
                    count++; //NEW DIEGO
                    if (count == k+1) { //NEW DIEGO
                        key = x;
                        count = 1; //NEW DIEGO
                        return;
                    }
                }
                else count = 1; //NEW DIEGO
                
                xp = iterators[p]->key();
                p = modulo(p+1,k);
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
                leapfrog_search_new(); //NEW DIEGO
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
                leapfrog_search_new(); //NEW DIEGO
            }
        }
        
        void leapfrog_seek_new(uint32_t seekKey){
            iterators[p]->seek(seekKey);
            if(iterators[p]->atEnd()) at_end = true;
            /*else{
                p = modulo(p+1,k);
                leapfrog_search_new(); //NEW DIEGO
            }*/
        }

        /*
            Moves iterators whos index in up_indicator is true. Used when only some of the iterators
            associated with this variable should go up.
            It doesn't move values horizontaly in the new level, it only sets them up so that it 
            the next LeapfrogJoin can use them
        */
        void up(map<uint32_t, bool> &tuple_should_go_up){
            for(auto it: iterators){
                if(tuple_should_go_up[it->getTuple()]){
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




            // for(int i=0; i<iterators.size(); i++){
            //     auto it = iterators[tuple_index];
            //     cout<<"este es el it de la tupla "<<it->getTuple();
            //     if(debug){cout<<"up indicator es "<<up_indicator[tuple_index]<<endl;}
            //     if(up_indicator[tuple_index]){
            //         if(debug){cout<<"solo subi"<<endl;}
            //         it->up();
            //     }
            //     else{
            //         if(debug){cout<<"subi y bajo"<<endl;}
            //         it->up();
            //         it->open();
            //     }
            // }
            // at_end = false;
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
            // cout<<"se hizo check_depths"<<endl;
            for(auto it: iterators){
                uint32_t index_tuple = it->getTuple();
                while(it->get_depth()>goal_depths[index_tuple]){
                    if(debug){cout<<"el depth "<<it->get_depth()<<" "<<goal_depths[index_tuple]<<endl;}
                    it->up();
                }
                if(debug){cout<<"la tupla "<<index_tuple<<" subio al nivel "<<it->get_depth();}
                // cout<<"la tupla "<<index_tuple<<" subio al nivel "<<it->get_depth()<<endl;
            }
        }
};


class LTJ{
    public:
        typedef std::pair<uint64_t, uint8_t> pair_type;
        typedef std::priority_queue<pair_type, std::vector<pair_type>, greater<pair_type>> min_heap_type;
        
    // private:
        //BORRAR
        bool debug = false;
        //HASTA AQUI
        vector<Iterator*> iterators;
        vector<Index*> *indexes;
        vector<Tuple> *query;
        vector<Tuple*> modified_query;
        // int tuple_index = 0;
        map<string, set<uint32_t>> *variable_tuple_mapping;
        map<string, LeapfrogJoin> variable_lj_mapping;
        std::vector<std::string>* processed_vars;
        bool at_end;
        uint32_t key;
        uint32_t depth;
        uint32_t dim;
        std::vector<std::string> m_gao;
        uint64_t number_of_vars;
        std::vector<uint8_t> tmp_gao;
        uint32_t limit;
        
        // Cosas para triejoin_tentativo
        bool show_results=false;

        std::unordered_map<std::string, std::vector<Iterator*>> m_var_to_iters;
        typedef struct {
            uint8_t id;
            uint64_t weight = -1UL;
            uint64_t n_triples;
            std::unordered_set<uint8_t> related;
            //Supports only variables with one occurence in a certain tuple.
            std::unordered_map<uint32_t, Iterator*> tuple_to_iter;
        } info_var_type;
        std::vector<info_var_type> m_var_info;
        std::unordered_map<uint8_t, uint64_t> m_hash_table_position;
        std::unordered_map<std::string, uint8_t> m_hash_table_vars;
        /*
        std::string get_next_var(std::string cur_var) const{
            return m_gao.at();
        }*/
        std::string get_gao() const{
            std::string ret = "";
            for(auto& var : m_gao){
                ret += "?" + var +" ";
            }
            return ret;
        }
        void clear(){
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
            Returns a string containing the variables that are in tuple, but ordered according to the gao 
            vector, it also adds the terms associated with each variable to the terms vector in the gao order
        */
        string getVariableOrder(Tuple &tuple, int &added_items, vector<Term> &terms){
            stringstream order;

            for(auto it=m_gao.begin(); it!=m_gao.end(); it++){
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

        bool is_var_subject(std::string var, Tuple &tuple){
            Term* term = tuple.get_term(0);
            if(term->isVariable() && term->varname == var){
                return true;
            }else{
                return false;
            }
        }
        
        bool is_var_predicate(std::string var, Tuple &tuple){
            Term* term = tuple.get_term(1);
            if(term->isVariable() && term->varname == var){
                return true;
            }else{
                return false;
            }
        }

        bool is_var_object(std::string var, Tuple &tuple){
            Term* term = tuple.get_term(0);
            if(term->isVariable() && term->varname == var){
                return true;
            }else{
                return false;
            }
        }

        // trim from end (in place)
        static inline void rtrim(std::string &s) {
            s.erase(std::find_if(s.rbegin(), s.rend(),
                    std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        }

        std::string get_order(Tuple& tuple, int index_to_exclude){
            std::stringstream order,variable_orders;
            for(int i=0; i<dim; i++){
                Term* term = tuple.get_term(i);
                if(!term->isVariable()){
                    order<<i<<" ";
                }else{
                    if(i != index_to_exclude){
                        variable_orders << i << " ";
                    }
                }
            }
            std::string orders_str = variable_orders.str();
            rtrim(orders_str);
            if(orders_str.length() > 0){
                order << index_to_exclude << " " << orders_str;
            }else{
                order << index_to_exclude;
            }
            
            return order.str();
        }

        void fill_heap(const uint8_t var,
                        std::unordered_map<uint8_t, uint64_t> &hash_table,
                        std::vector<info_var_type> &vec,
                        std::vector<bool> &checked,
                        min_heap_type &heap){

            auto pos_var = hash_table[var];
            for(const auto &e : vec[pos_var].related){
                auto pos_rel = hash_table[e];
                if(!checked[pos_rel] && vec[pos_rel].n_triples > 1){
                    heap.push({vec[pos_rel].weight, e});
                    checked[pos_rel] = true;
                }
            }
        }

        struct compare_var_info
        {
            inline bool operator() (const info_var_type& linfo, const info_var_type& rinfo)
            {
                if(linfo.n_triples>1 && rinfo.n_triples==1){
                    return true;
                }
                if(linfo.n_triples==1 && rinfo.n_triples>1){
                    return false;
                }
                return linfo.weight < rinfo.weight;
            }
        };
        void update_hash_var_index(typename std::vector<info_var_type>::iterator it_start, typename std::vector<info_var_type>::iterator it_end, std::unordered_map<uint8_t, uint64_t> &hash_table){
            int i=0;
            typename std::vector<info_var_type>::iterator it = it_start;
            while(it != it_end){
                auto hash_it = hash_table.find(it->id);
                if(hash_it != hash_table.end()){
                    hash_it->second = i;
                    i++;
                }
                it = std::next(it);
            }
        }
        void var_to_related(uint8_t var, uint8_t rel,
                            std::unordered_map<uint8_t, uint64_t> &hash_table,
                            std::vector<info_var_type> &vec){
            auto pos_var = hash_table[var];
            vec[pos_var].related.insert(rel);
            auto pos_rel = hash_table[rel];
            vec[pos_rel].related.insert(var);
        }

        std::vector<std::string> calculate_gao(){
            //***************************************************GAO
            /*Algoritmo:
            b. Obtengo el orden parcial con las constantes (si es que hay).
                
                Supongamos que estamos trabajando con el triple ?X1 100 ?X2, entonces:
                order<<getConstantsOrder(tuple, added_items, terms);

                deja order = "1 "

            c. Completamos el orden parcial como sigue:
                Si estamos evaluando X1, entonces tenemos que usar el iterator 1 0 2 (como trie primero parto por P, sigo por S y termino con O) .
                Para x2 usamos la permutación 1 2 0.

            d. Creamos un iterador por cada variable en una tupla, utilizando el orden calculado en (c.).
            e. Calculamos el gao como sigue:
                e1. Sea Xj una variable del BGP. bajamos por cada uno de los iteradores de Xj, procesando las constantes tal como sale en el código actual, bajo el comentario:
                //Resolvemos las constantes para todas las tuplas") en triejoin_definitivo() 

                e2. Calculo el número de hijos para todos los nodos de los iteradores de Xj, y me quedo con el mínimo.
                
                * Adicionalmente, la instanciación de variables tiene que generar un grafo conexo (en  la medida de lo posible).
            */
           //>>*************************************************GAO
            m_var_info.clear();
            m_hash_table_position.clear();
            m_var_to_iters.clear();
            m_hash_table_vars.clear();
            vector<Iterator*> gao_iterators;
            //Creating the m_var_info entries.
            for(auto it=query->begin(); it!=query->end(); it++){
                Tuple &tuple = *it;
                for(int i=0; i<dim; i++){
                    Term* term = tuple.get_term(i);
                    if(term->isVariable()){

                        auto var = term->varname;
                        auto it = m_hash_table_vars.find(var);
                        if(it == m_hash_table_vars.end()){
                            uint8_t id = m_hash_table_vars.size();
                            m_hash_table_vars.insert({var, id });
                            info_var_type info;
                            info.id = id;
                            info.n_triples = 0;
                            m_var_info.emplace_back(info);
                            m_hash_table_position.insert({info.id, m_var_info.size()-1}); 
                        }
                    }
                }
            }//Se crean los iteradores por cada tupla t_{X_i} de cada variable Xi.
            for(auto it=variable_tuple_mapping->begin(); it!=variable_tuple_mapping->end(); it++){
                auto &p = *it;
                std::string var = p.first;

                auto id_it = m_hash_table_vars.find(var);
                uint8_t tuple_id = id_it->second;
                info_var_type& info = m_var_info[m_hash_table_position.at(tuple_id)];
                info.n_triples = p.second.size();
                //b.
                for(auto& tuple_index : p.second){
                    std::string order;
                    auto& tuple = query->at(tuple_index);
                    //c.
                    if(is_var_subject(var, tuple)){
                        //order << getConstants() << 0 << getVars(0)
                        order = get_order(tuple, 0);
                    }else if(is_var_predicate(var, tuple)){
                        //order << getConstants() << 1 << getVars(1)
                        order = get_order(tuple, 1);
                    }else{
                        //order << getConstants() << 2 << getVars(2)
                        order = get_order(tuple, 2);
                    }
                    //d.
                    auto iter = new CurrentIterator(indexes->at(0)->getTrie(order), tuple_index);
                    //std::cout << "Variable '" << var << "' : a new iterator using order '" << order << "' for tuple number " << tuple_index << " is created."<<std::endl;
                    iter->open();
                    gao_iterators.push_back(iter);
                    m_var_to_iters[var].push_back(iter);
                    info.tuple_to_iter[tuple_index] = iter;
                }

            }
            //e. Por cada variable x_i, procesamos las constantes de cada uno de sus iteradores que corresponden a las tuplas t_{x_i}.
            for(auto it=variable_tuple_mapping->begin(); it!=variable_tuple_mapping->end(); it++){
                auto &aux = *it;
                std::string var = aux.first;//the variable
                //  e1.
                //std::cout << "Calculating variable's weight and also, per each regular variable finding its relative."<< std::endl;
                for(auto tuple_index : variable_tuple_mapping->at(var)){//all the tuples in which the variable participates.
                    uint8_t var_id = m_hash_table_vars[var];
                    info_var_type& info = m_var_info[m_hash_table_position.at(var_id)];
                    Iterator* iter = info.tuple_to_iter[tuple_index];
                    Tuple &tuple = query->at(tuple_index);

                    bool s = false, p = false, o = false;
                    uint8_t var_s, var_p, var_o;

                    for(int j=0; j<dim; j++){
                        Term* term = tuple.get_term(j);
                        if(!term->isVariable()){
                            iter->seek(term->getConstant());
                            //e2.
                            auto children_count = iter->getChildrenCount();
                            if(info.weight > children_count)
                                info.weight = children_count;  
                            //std::cout <<  "Var : " << int(info.id) << " num of children : " << children_count << " on tuple " << tuple_index << std::endl;        
                            if(iter->atEnd() || iter->key() != term->getConstant()){
                                // Si es que el valor no es igual a la constante entonces no 
                                // hay valores que cumplan esta tupla
                                break;
                            }
                        }else{
                            //if 'j' entry is variable then we'll mark it.
                            if(j == 0){
                                s = true;
                                var_s = m_hash_table_vars[term->varname];
                            }
                            else if(j == 1){
                                p = true;
                                var_p = m_hash_table_vars[term->varname]; 
                            }
                            else{
                                o = true;
                                var_o = m_hash_table_vars[term->varname];  
                            }
                        }
                    }
                    //rel variables.
                    if(s && p){
                        var_to_related(var_s, var_p, m_hash_table_position, m_var_info);
                    }
                    if(s && o){
                        var_to_related(var_s, var_o, m_hash_table_position, m_var_info);
                    }
                    if(p && o){
                        var_to_related(var_p, var_o, m_hash_table_position, m_var_info);
                    }
                }
            }
            
            //Sorting by compare_var_info()
            std::sort(m_var_info.begin(), m_var_info.end(), compare_var_info());
            update_hash_var_index(m_var_info.begin(), m_var_info.end(), m_hash_table_position);
            uint64_t lonely_start = m_var_info.size();
            for(uint64_t i = 0; i < m_var_info.size(); ++i){
                m_hash_table_position[m_var_info[i].id] = i;
                if(m_var_info[i].n_triples == 1 && i < lonely_start){
                    lonely_start = i;
                }
            }
    
            //Calculate the GAO based upon the sorted variables.
            std::vector<bool> checked(m_var_info.size(), false);
            for(uint64_t i = 0; i < lonely_start; i++){
                if(!checked[i]){
                    tmp_gao.push_back(m_var_info[i].id); //Adding var to gao
                    checked[i] = true;
                    min_heap_type heap; //Stores the related variables that are related with the chosen ones
                    auto var_id = m_var_info[i].id;
                    fill_heap(var_id, m_hash_table_position, m_var_info, checked,heap);
                    while(!heap.empty()){
                        var_id = heap.top().second;
                        heap.pop();
                        tmp_gao.push_back(var_id);
                        fill_heap(var_id, m_hash_table_position, m_var_info, checked, heap);
                    }
                }
            }
            for(uint64_t i = lonely_start; i < m_var_info.size(); i++){
                tmp_gao.push_back(m_var_info[i].id);
            }

            //--
            for(auto gao_it: gao_iterators){
                delete gao_it;
            }

            //tmp_gao = {'\000', '\003', '\004', '\002', '\001'};//RING
            //tmp_gao = {'\001', '\000', '\003', '\002', '\004'};//RING MUTHU
            //Finally we transform from the internal (temp) gao which uses uint8_t required to use the min_heap properly (test with strings, it wont work fine, lexicographical sorting is not the same for int8_t and strings).
            std::unordered_map<uint8_t, std::string> ht;
            for(const auto &p : m_hash_table_vars){
                ht.insert({p.second, p.first});
            }
            
            std::vector<std::string> gao;
            for(uint8_t var_id : tmp_gao){
                auto str = ht[var_id];
                gao.emplace_back(str);
            }
            //<<***************************************************GAO
            number_of_vars = gao.size();
            return gao;
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
                //aqui recien necesito el gao.
                order<<getVariableOrder(tuple, added_items, terms);//********
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
        LTJ(vector<Index*> *ind, vector<Tuple> *q, map<string, set<uint32_t>> *variables_to_index, uint32_t lmt, std::vector<std::string>* processed_vars){
            this->indexes = ind;
            this->query = q;
            dim = indexes->at(0)->getDim();
            this->variable_tuple_mapping = variables_to_index;
            this->processed_vars = processed_vars;
            m_gao = calculate_gao();
            setIterators();
            createLeapfrogJoins();
            depth = 0;
            
            limit = lmt;
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
            for(int i=0; i<number_of_vars; i++){
                if(m_gao.at(i)==var)return i;
            }
            return -1;
        }
        
        /*
            Returns a vector indicating if each of the iterators associated with the variable should, 
            or can go up a level
        */
        void check_for_prev_value(string &var, int &gao_score, map<uint32_t, bool> &tuple_should_go_up){
            if(debug){cout<<"cheching for prev value "<<var<<" "<<gao_score<<endl;}

            
            // vector<bool> should_go_up;
            for(auto tuple_index : variable_tuple_mapping->at(var)){
                Tuple *tuple = modified_query[tuple_index];
                int term_index = get_var_index_in_tuple(tuple, var);
                if(debug){cout<<"term_index es "<<term_index<<endl;}
                if(term_index == 0){
                    tuple_should_go_up[tuple_index] = false;
                    // should_go_up.push_back(false);
                }
                else{
                    Term *prev_term = tuple->get_term(term_index-1);
                    if(!prev_term->isVariable()){
                        tuple_should_go_up[tuple_index] = false;
                        // should_go_up.push_back(false);
                    }
                    else{
                        //Si el termino previo es una variable con < gao score que el que buscamos
                        string prev_var = prev_term->getVariable();
                        if(get_gao_score(prev_var) < gao_score){
                            tuple_should_go_up[tuple_index] = false;
                            // should_go_up.push_back(false);
                        }
                        else{
                            tuple_should_go_up[tuple_index] = true;
                            // should_go_up.push_back(true);
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
                string var = m_gao.at(i);
                if(debug){cout<<"Going up on var "<<var<<endl;}
                LeapfrogJoin* lj = &variable_lj_mapping[var];
                // vector<bool> should_go_up;
                map<uint32_t, bool> tuple_should_go_up;
                // vector<bool> should_go_up = check_for_prev_value(var, gao_score);
                check_for_prev_value(var, gao_score,tuple_should_go_up);
                if(debug){cout<<"cheching should go up"<<endl;}
                // if(debug){
                //     for(auto v: should_go_up){
                //         cout<<v<<" ";
                //     }
                //     cout<<endl;
                // }
                if(debug){cout<<"en el up de goUpUntil "<<endl;}
                lj->up(tuple_should_go_up);
                // lj->up();
                gao_index--;
            }

            // cout<<"depths after going up for"<<endl;
            // for(auto it: iterators){
            //     cout<<it->get_depth()<<" ";
            // }
            // cout<<endl;
            // if(debug){
            //     cout<<"Iterators positions and keys:"<<endl;
            //     for(auto it: iterators){
            //         cout<<"depth: "<<it->get_depth()<<"/ key: "<<it->key()<<endl;
            //     }
            // }
            if(debug){cout<<"gao score is "<<gao_score<<" "<<m_gao.at(gao_score)<<endl;}
            LeapfrogJoin* lj = &variable_lj_mapping[m_gao.at(gao_score)];
            if(debug && lj->is_at_end()){
                cout<<"el iterador ya estaba en at end"<<endl;
            }
            check_iterators_position(lj, m_gao.at(gao_score));
            if(debug)cout<<"se hace next para "<<m_gao.at(gao_score)<<endl;
            lj->leapfrog_next();
            if(lj->is_at_end()){
                if(debug){cout<<"el iterador esta at en en goUpUntil"<<endl;}
                if(gao_score==0){
                    if(debug){cout<<"Cant go up"<<endl;}
                    return true;
                }
                if(debug){cout<<"going up again on "<<m_gao.at(gao_index)<<" index "<<gao_index<<endl;}
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
            vector<int> result(number_of_vars);
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
                        // cout<<"se movió iterador "<<i<<" a "<<iterators[tuple_index]->key()<<endl;
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
            while(gao_index < number_of_vars && !finished){
                // cout<<"depths before search"<<endl;
                // for(auto it: iterators){
                //     cout<<it->get_depth()<<" ";
                // }
                // cout<<endl;
                string var = m_gao.at(gao_index);
                if(debug){cout<<"buscando para var "<<var<<endl;}
                LeapfrogJoin* lj = &variable_lj_mapping[var];
                if(debug){cout<<"Se encontró LJ para "<<var<<endl;}
                lj->leapfrog_search_new();  //NEW DIEGO
                if(debug){cout<<"Se hizo search"<<endl;}
                int current_level = gao_index;
                while(current_level == gao_index){
                    if(lj->is_at_end()){
                        if(gao_index == 0){
                            number_of_results = count;
                            return;
                        }
                        else {
                            if(debug){cout<<"going up"<<endl;}
                            finished = goUp(gao_index);
                        }
                    }
                    else{
                        // cout<<"gao index es "<<gao_index<<endl;
                        result[gao_index] = lj->get_key();
                        if(debug){cout<<var<<": "<<lj->get_key()<<endl;}
                        if(gao_index == number_of_vars-1){
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
                for(auto it=m_gao.begin(); it!=m_gao.end(); it++){
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
