#include <iostream>
#include "leapfrog_trie_join.hpp"
#include "table_indexing.hpp"
#include "index.hpp"
#include "utils.hpp"
#include "term.hpp"
#include "tuple.hpp"
#include <vector>
#include <set>
using namespace std;


int main(int argc, char* argv[]){
    try{
        vector<string> queries;
        vector<vector<string>> gaos;
        bool have_queries =  get_file_content(argv[1], queries, gaos);
        // string query1 = "?x1 ?x2 ?x3";
        // Debería hacerse un index por argc sin contar a las queries
        TableIndexer ti = TableIndexer();
        // Index index1 = ti.indexNewTable(argv[2]);
        Index index1 = ti.loadIndex(argv[2]);

        uint64_t limit = -1;
        if(argc > 3){
            limit = stoi(argv[3]);
        }
        // Index index2 = ti.indexNewTable(argv[3]);

        // vector<Index*> indexes = {&index1, &index2};
        vector<Index*> indexes = {&index1};
        uint64_t query_number = 1;
        if(have_queries){
            for(string query_string : queries){  
                cout<<"Query "<<query_number<<":"<<endl;
                cout<<"GAO: ";
                for(auto var: gaos[query_number-1]){
                    cout<<var<<" ";
                }cout<<endl;
                vector<Term*> terms_created;
                // Guarda en que tuplas se encuentra cada variable
                map<string, set<uint64_t>> variable_tuple_mapping;
                vector<Tuple*> query;

                vector<string> tokens_query = parse(query_string, '.');
                
                
                uint64_t term_index = 0;
                for(string token : tokens_query){
                    query.push_back(get_tuple(token, variable_tuple_mapping, term_index));
                    term_index++;
                }

                //En vez de tener terms created, en variable_mapping vamos a guardar las variables y los indices

                LTJ ltj(indexes, query, gaos[query_number-1], variable_tuple_mapping, limit);
                cout<<"Constructor works"<<endl;
                ltj.triejoin_definitivo();
                query_number++; 
                cout<<endl<<endl<<endl;
            }
        }  
    }
    catch(const char *msg){
        
        cerr<<"ERROR DURING QUERY:"<<msg<<endl;
    }
}