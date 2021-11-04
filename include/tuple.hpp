#ifndef TUPLE_H
#define TUPLE_H
#include "term.hpp"
#include <vector>

class Tuple {
    private:
        vector<Term*> terms;
    public:
        /*
            Empty Constructor
        */
        Tuple();

        /*
            Constructor recives a vector of terms 
        */
        Tuple(vector<Term*> t){
            terms = t;
        }

        /*
            Sets the terms of the tuple to t
        */
        void set_terms(vector<Term*> t){
            terms = t;
        }

        /*
            Returns the term with the index i in the terms associated with this tuple
        */
        Term* get_term(uint64_t i){
            return terms[i];
        }
};
#endif
