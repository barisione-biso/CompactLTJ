#include <iostream>
#include "leapfrog_trie_join.hpp"
#include "table_indexing.hpp"
#include "index.hpp"
#include "utils.hpp"
#include "term.hpp"
#include "tuple.hpp"
#include <vector>
using namespace std;


int main(int argc, char* argv[]){
    try{
        vector<string> queries;
        bool have_queries =  get_file_content(argv[1], queries);
        // string query1 = "?x1 ?x2 ?x3";

        // Debería hacerse un index por argc sin contar a las queries
        TableIndexer ti = TableIndexer();
        Index index1 = ti.indexNewTable(argv[2]);
        Index index2 = ti.indexNewTable(argv[3]);

        vector<Index*> indexes = {&index1, &index2};
        uint64_t query_number = 1;
        if(have_queries){
            for(string query_string : queries){  
                cout<<"Query "<<query_number<<":"<<endl;
                query_number++; 
                vector<Term*> terms_created;
                map<string, uint64_t> variable_index_mapping;
                vector<Tuple*> query;

                vector<string> tokens_query = parse(query_string, '.');
                
                uint64_t i = 0;
                for(string token : tokens_query){
                    query.push_back(get_tuple(token, variable_index_mapping, i));
                }

                //En vez de tener terms created, en variable_mapping vamos a guardar las variables y los indices

                LTJ ltj(indexes, query, variable_index_mapping);
                ltj.triejoin();
                ltj.evaluate();
            }
        }  
    }
    catch(const char *msg){
        cerr<<msg<<endl;
    }
}