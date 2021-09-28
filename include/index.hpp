#ifndef INDEX_H
#define INDEX_H

#include <iostream>
#include "iterator.hpp"
#include <filesystem>

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

        /*
            Saves index representation in folder with the same name of the entry file
            
        */
        void save(){
            fs::create_directory(folder);

            ofstream stream(folder+"info.txt");
            if(stream.is_open()){
                stream<<"orders: ";
                for(auto order: orders){
                    stream<<order<<", ";
                }
            }
            stream.close();

            u_int64_t i = 0;
            for(auto it: iterators){
                it->storeToFile(folder+"order"+to_string(i)+".txt");
                i++;
            }
        }
};

#endif