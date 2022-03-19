#ifndef TUPLE_H
#define TUPLE_H
#include "term.hpp"
#include <vector>

class Tuple {
    public:
    // private:
        vector<Term> terms;
        bool modified = false;
    // public:
        /*
            Empty Constructor
        */
        Tuple();

        ~Tuple(){
            // if(modified){
            //     for(auto term : terms){
            //         delete term;
            //     }
            // }
        };

        /*
            Constructor recives a vector of terms 
        */
        Tuple(vector<Term> t){
            terms = t;
        }

        /*
            Sets the terms of the tuple to t
        */
        void set_terms(vector<Term> t){
            terms = t;
        }

        /*
            Returns the term with the index i in the terms associated with this tuple
        */
        Term* get_term(uint64_t i){
            return &terms[i];
        }

        /*
            Prints values asociated to the current tuple    
        */
        void printTuple(){
            for(auto t: terms){
                t.getValues();
                cout<<" ";
            }
            // cout<<endl;
        }
};
#endif
