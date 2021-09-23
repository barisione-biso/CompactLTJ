#include <iostream>
#include "table_indexing.hpp"
#include "index.hpp"

using namespace std;

int main(int argc, char **argv){
    try{
        
        if(argc <= 1){
            cout<<"No extra command line argument given other that program name"<<endl;
            return 0;
        }
        string file_name = argv[1];

        /*
            Aqui llamar a table indexer que luego retornará un index
            Se podrían indexar varias tablas al mismo tiempo
            todos los archivos que acompañen a ./build_index
        */

        TableIndexer ti = TableIndexer();
        ti.indexNewTable(file_name);
        Index ind = ti.indexNewTable(file_name);
        ind.save();
    }
    catch(const char *msg){
        cerr<<msg<<endl;
    }
    return 0;
}