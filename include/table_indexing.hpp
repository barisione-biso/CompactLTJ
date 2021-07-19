#ifndef TABLE_INDEXING_H
#define TABLE_INDEXING_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <map>
#include "regular_trie.hpp"


using namespace std;


class TableIndexer{
    private:

    vector<vector<uint64_t> > table;
    vector<string> orders;
    uint64_t dim = 0;
    bool all_orders = false;
    Trie* root;
    bit_vector B;
    string S;
    CompactTrieIterator compactTrie;
    

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

    /*
        Turns uint64_t vector in to bitvector B 
    */
    void toBitvector(vector<uint64_t> &b){
        B = bit_vector(b.size(), 0);

        for(int i=0; i<b.size(); i++){
            B[i]=b[i];
        }
    }

    /*
        Turns uint64_t vector in to string S
    */
    void toSequence(vector<uint64_t> &s){
        ostringstream stream;
        for(int i=0; i<s.size(); i++){
            stream<<s[i]<<" ";
        }

        S = stream.str();
    }
    public:

    TableIndexer(){
        cout<<"Table Indexer creado"<<endl;
    }

    /*
        Creates a traditional trie with the table contents
    */
    void createRegularTrie(){
        root = new Trie();
        Trie* node;

        /*
        For the moment we asume that the Trie that we are creating is the one that
        comes with the order from the table but in the future we will recibe the order
        or something equivalent to index all orders necessary
        */

        for(int j=0; j<table[0].size(); j++){
            node = root;
            for(int i=0; i<table.size(); i++){
                node = node->insert(table[i][j]);
            }
        }
    }

    /*
        Turns Trie into a bitvector B and a sequence S
    */
    void toCompactForm(){
        vector<uint64_t> b;
        vector<uint64_t> s;
        map<uint64_t, Trie*> node_children;
        queue<Trie*> q;
        Trie* node;

        q.push(root);
        b.push_back(1);
        b.push_back(0);

        while(!q.empty()){
            node = q.front();
            q.pop();
            if(node->hasChildren()){
                node_children = node->getChildren();
                for(const auto &child: node_children){
                    s.push_back(child.first);
                    b.push_back(1);
                    q.push(child.second);
                }   
            }
            b.push_back(0);
        }

        toBitvector(b);
        toSequence(s);
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
                vector<string> line_values = parse(line, ' ');

                for(int i=0; i<dim; i++){
                    table[i].push_back(stoi(line_values[i]));
                }
            }
        }
        createRegularTrie();
        toCompactForm();
        compactTrie = CompactTrieIterator(B, S);
    }
    
};

#endif