#ifndef TABLE_INDEXING_H
#define TABLE_INDEXING_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include "regular_trie.hpp"

#include <sdsl/vectors.hpp>
#include "iterator.hpp"
#include "compact_trie_iterator.hpp"
#include "index.hpp"

using namespace std;


using namespace sdsl;

class TableIndexer{
    private:

    vector<vector<uint64_t> > table;
    vector<string> orders;
    uint64_t dim;
    bool all_orders;
    Trie* root;
    bit_vector B;
    string S;
    vector<Iterator *> compactTries;
    
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

        for(auto &val: s){
            stream<<val<<" ";
        }
        S = stream.str();
    }

    /*
        Creates a traditional trie with the table contents following the order in index
    */
    void createRegularTrie(map<u_int64_t, u_int64_t> index){
        Trie* node;

        for(int j=0; j<table[0].size(); j++){
            node = root;
            for(int i=0; i<table.size(); i++){
                node = node->insert(table[index[i]][j]);
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
        Creates indexes for all the orders necessary 
    */
    void createIndexes(){
        for(auto &value: orders){
            vector<string> order = parse(value, ' ');
            map<u_int64_t, u_int64_t> index;
            for(int j=0; j<order.size(); j++){
                index[j] = stoi(order[j]);
            }
            root = new Trie();
            createRegularTrie(index);
            toCompactForm();
            compactTries.push_back(new CompactTrieIterator(B, S));
            delete root;
        }
    }

    void clearData(){
        table.clear();
        orders.clear();
        dim = 0;
        all_orders = false;
        for(auto p : compactTries){
            delete p;
        }
        compactTries.clear();
    }
    public:

    TableIndexer(){}

    /*
        Recives a file with the table that needs to be indexed.
        First line of the file indicates the dimensions of the table
        Second line of the file indicates which orders need to be indexed.
    */
    Index indexNewTable(string file_name){
        clearData();
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
            else{
                throw "File doesn't follow format:\n\
                 * First line should indicate dimension of the table -> dim:n \n\
                 * Second line should indicate which orders need to be indexed -> orders: 0 1 2, 1 2 0 \n\
                 * If all the orders need to be indexed then use -> orders: \n\
                 * The rest of the file should have the table, all the lines should have the same amount \n\
                 of values and it should be equal to dim. ";
            }
        }
        reader.close();

        if(all_orders){
            vector<uint64_t> rows(dim);
            ostringstream stream;
            for(int i=0; i<dim; i++){
                rows[i] = i;
            }

            do{
                stream.str("");
                for(auto &value: rows){
                    stream<<value<<" ";
                }
                orders.push_back(stream.str());
            }while(next_permutation(rows.begin(), rows.end()));
        }
        createIndexes();
        // compactTrie.store_to_file();
        Index ind(orders, compactTries, file_name);
        return ind;
    } 
};

#endif