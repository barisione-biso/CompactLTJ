#include <iostream>
#include "table_indexing.hpp"
#include "index.hpp"

using namespace std;

using namespace std::chrono;
using timer = std::chrono::high_resolution_clock;

int main(int argc, char **argv){
    try{
        
        if(argc <= 1){
            cout<<"No extra command line argument given other that program name"<<endl;
            return 0;
        }
        

        TableIndexer ti = TableIndexer();
        

        // for(int i=1; i<argc; i++){
            
        // }

        string file_name = argv[1];
        ti.readTable(file_name);
        auto start = timer::now();

        ti.indexNewTable(file_name);
        auto stop = timer::now();

        ti.saveIndex();
        
        cout << "Index saved" << endl;
        cout << duration_cast<seconds>(stop-start).count() << " seconds." << endl;
        // ti.indexNewTable(file_name);
        
        // ind.save();
    }
    catch(const char *msg){
        cerr<<msg<<endl;
    }
    return 0;
}