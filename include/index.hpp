#ifndef INDEX_H
#define INDEX_H

#include <iostream>
#include "iterator.hpp"
#include <filesystem>
#include "utils.hpp"

using namespace std;
namespace fs = std::filesystem;

class Index{
    private:
        vector<string> orders;
        vector<Iterator*> iterators;
        string folder = "../data/";

    public:
        Index(vector<string> ord, vector<Iterator*> its, string file_name){
            orders = ord;
            iterators = its;
            /* The folder where the index file will be saved will be in the data folder 
            with the name of the file that was indexed */
            uint64_t s = file_name.size();
            folder = file_name.substr(0, s-4) + "/";
        }

        Index(string folder_name){
            folder = folder_name;
            load();
        }

        /*
            Saves index representation in folder with the same name of the entry file
        */
        void save(){
            fs::create_directory(folder);
            bool first = true;

            ofstream stream(folder+"info.txt");
            if(stream.is_open()){
                stream<<"orders: ";
                for(auto order: orders){
                    stream<<order;
                    if(!first){
                        stream<<", ";
                    }
                    first = false;
                }
            }
            stream.close();

            u_int64_t i = 0;
            for(auto it: iterators){
                it->storeToFile(folder+"order"+to_string(i)+".txt");
                i++;
            }
        }

        /*
            Loads index representation from folder
        */
        void load(){
            string line;
            ifstream stream(folder + "info.txt");
            if(stream.is_open()){
                getline(stream, line);
                if(line.substr(0, 7) == "orders:"){
                    orders = parse(line.substr(7), ',');
                }
                else{
                    throw "Info file from " + folder + " index doesn't have the appropiate format";
                }
            }
            stream.close();

            for(int i=0; i<orders.size(); i++){
                iterators.push_back(new CompactTrieIterator(folder+"order"+to_string(i)+".txt"));
            }
        }
};

#endif