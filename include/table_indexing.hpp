#ifndef TABLE_INDEXING_H
#define TABLE_INDEXING_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "regular_trie.hpp"

using namespace std;

class TableIndexer{
    private:

    vector<vector<uint64_t> > table;
    vector<string> orders;
    uint64_t dim = 0;
    bool all_orders = false;

    /*
    Parses string (line) by a single char (delimiter)
    Returns vector with all the parts of the parsed string 
    */
    vector<string> parse(string line, char delimiter){
        uint64_t first = 0;
        vector<string> results;

        for(int i=0; i<line.size(); i++){
            if(line[i] == delimiter && first!=i){
                results.push_back(line.substr(first, i-first));
                first = i+1;
            }
        }

        if(first !=line.size()){
            results.push_back(line.substr(first, line.size()-first));
        }

        return results;
    }

    public:

    TableIndexer(){
        cout<<"Table Indexer creado"<<endl;
    } 

    void createRegularTrie(){
        Trie* root = new Trie();
        Trie* node;

        /*
        For the moment we asume that the Trie that we are creating is the one that
        comes with the order from the table but in the future we will recibe the order
        or something equivalent to index all orders necessary
        */

        u_int64_t filas = table.size();
        u_int64_t columnas = table[0].size();

        //Agregamos columna por columna 
        for(int j=0; j<table[0].size(); j++){
            node = root;
            for(int i=0; i<table.size(); i++){
                node->insert(table[j][i]);
                // Falta tomar el nodo que da y a ese agregarle el que sigue
            }
        }
    }
    
    /*
        Recives a file with the table that needs no be indexed.
        First line of the file indicates the dimensions of the table
        Second line of the file indicates which orders need to be indexed.
    */
    void indexTable(string file_name){
        if(file_name.substr(file_name.size()-4, 4) != ".txt") throw "File for indexing must have .txt extension";
        
        ifstream reader(file_name);
        string line;
        bool first_line = true;
        bool second_line = false;
        u_int64_t value;
        u_int64_t num = 0;

        while(reader.is_open() && getline(reader, line)){
            if(first_line && line.substr(0,4) == "dim:"){
                dim = stoi(line.substr(4));
                table.resize(dim);
                first_line = false;
                second_line = true;
            }
            else if(second_line && line.substr(0, 7) == "orders:"){
                orders = parse(line.substr(7), ',');
                if(orders.size() == 0) all_orders = true;
                second_line = false;
            }
            else if(dim!=0 && (all_orders || orders.size()!=0)){
                num = 0;
                do{
                    reader>>value;
                    table[num].push_back(value);
                    num++;
                } while(num<dim);
            }
        }
        cout<<"Saved Table"<<endl;
        createRegularTrie();
    }

    
};

#endif