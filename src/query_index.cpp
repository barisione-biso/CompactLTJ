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

        if(have_queries){
            for(string query_string : queries){   

                vector<Term*> terms_created;
                vector<Tuple*> query;

                vector<string> tokens_query = parse(query_string, '.');

                for(string token : tokens_query){
                    query.push_back(get_tuple(token, terms_created));
                }

                LTJ ltj(indexes, query);
                ltj.triejoin();
            }
        }  
    }
    catch(const char *msg){
        cerr<<msg<<endl;
    }
}