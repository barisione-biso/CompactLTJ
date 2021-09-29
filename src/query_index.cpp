#include <iostream>
#include "leapfrog_trie_join.hpp"
#include "table_indexing.hpp"
#include "index.hpp"

using namespace std;


int main(int argc, char* argv[]){
    string query = "?x1 ?x2 ?x3";

    TableIndexer ti = TableIndexer();
    Index index1 = ti.indexNewTable(argv[1]);
    Index index2 = ti.indexNewTable(argv[2]);

    vector<Index*> indexes = {&index1, &index2};

    LTJ ltj(indexes);


}