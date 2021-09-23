#ifndef INDEX_H
#define INDEX_H

#include <iostream>
#include "iterator.hpp"

using namespace std;

class Index{
    private:
        uint64_t dim;
        string file_name;
        vector<string> orders;
        vector<Iterator*> iterators;

    public:
        Index(vector<string> orders, vector<Iterator*> iterators, string file_name){
            orders = orders;
            iterators = iterators;
            file_name = file_name;
        }

        void save(){
            cout<<"Saving"<<endl;
            // for(int i=0; i<orders.size(); i++){

            // }
        }
};

#endif