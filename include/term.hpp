#ifndef TERM_H
#define TERM_H

#include <string>
#include <iostream>
using namespace std;

class Term {
    private: 
        bool is_variable;
        string varname;
        uint64_t constant;
    public:
        /*
            Empty constructor
        */
        Term();

        /*
            Constructor for constant terms
        */
        Term(uint64_t c){
            is_variable = false;
            constant = c;
        }

        /*
            Constructor for variables
        */
        Term(string v){
            is_variable = true;
            varname = v;
        }

        /*
            Prints the values asociated with this term
        */
        void getValues(){
            if(is_variable)cout<<varname<<endl;
            else cout<<constant<<endl;
        }

        /*
            Returns true if the term is a variable
        */
        bool isVariable(){
            return is_variable;
        }

        /*
            Returns value asociated to the constant variable.
            Before calling it, it should be checked that the term is not a variable
        */
        uint64_t getConstant(){
            return constant;
        }
        
        /*
            Returns the variable name of the current term 
            Before calling it, it should be checked that the term is a variable
        */
        string getVariable(){
            return varname;
        }
};


#endif 
