#ifndef TABLE_INDEXING_H
#define TABLE_INDEXING_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

class TableIndexer{
    private:
    /*
    Parses string (line) by a single char (delimiter)
    Returns vector with all the parts of the parsed string 
    */
    vector<string> parse(string line, char delimiter){
        int first = 0;
        vector<string> results;

        for(int i=0; i<line.size(); i++){
            if(line[i] == delimiter && first!=i){
                result.push_back(line.substr(first, i-first));
                first = i+1;
            }
        }

        if(first !=line.size()){
            result.push_back(line.substr(first, line.size()-first));
        }
        
        return results;
    }

    public:

    TableIndexer(){
        cout<<"Table Indexer creado"<<endl;
    } 
    
    void indexTable(string file_name){
        
        if(file_name.substr(file_name.size()-4, 4) != ".txt") throw "File for indexing must have .txt extension";
        
        ifstream reader(file_name);
        string line;
        bool first_line = true;
        bool all_orders = false;
        vector<string> orders;

        while(reader.is_open() && getline(reader, line)){
            if(first_line){
                if(line.substr(0, 7) == "orders:"){
                    //parse orders separeted by comas 
                }
                else{
                    all_orders =  true;
                }
            }
        }

    }
};

#endif