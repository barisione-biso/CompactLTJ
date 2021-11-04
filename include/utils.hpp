#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <sstream> 
#include "term.hpp"
#include "tuple.hpp"

using namespace std;
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
    Removes leading and trailing characters that are equal to to_cut
*/
string trim(string s, char to_cut){
    u_int64_t beg=0, end=s.size();
    for(int i=0; i<s.size(); i++){
        if(s[i]==to_cut)beg++;
        else break;
    }
    for(int i=s.size()-1; i>=0; i--){
        if(s[i]==to_cut)end--;
        else break;
    }
    return s.substr(beg, end-beg);
}

/*
    Recives contents of file and saves each line in a vector of strings
*/
bool get_file_content(string filename, vector<string> & vector_of_strings)
{
    // Open the File
    ifstream in(filename.c_str());
    // Check if object is valid
    if(!in)
    {
        cerr << "Cannot open the File : " << filename << endl;
        return false;
    }
    string str;
    // Read the next line from File untill it reaches the end.
    while (getline(in, str))
    {
        // Line contains string of length > 0 then save it in vector
        if(str.size() > 0)
            vector_of_strings.push_back(str);
    }
    //Close The File
    in.close();
    return true;
}

/*
    Checks that all elements in s are digits, in turn discovering if s is a number
*/
bool is_number(string & s)
{
    return !s.empty() && find_if(s.begin(),
        s.end(), [](unsigned char c) { return !isdigit(c); }) == s.end();
}

/*
    Returns a tuple consisting of the Terms that are found in s. 
    Also adds the new terms to the terms vector
*/
Tuple* get_tuple(string &s, vector<Term*> & terms){
    vector<string> terms_string = parse(trim(s, ' '), ' ');
    vector<Term*> new_terms;
    u_int64_t value;

    for(auto s: terms_string){
        // cout<<"term "<<terms_string[i]<<endl;
        if(is_number(s)){
            istringstream iss(s);
            iss >> value;
            new_terms.push_back(new Term(value));
        }
        else{
            new_terms.push_back(new Term(s));
        }
    }

    for(auto term: new_terms){
        terms.push_back(term);
    }
    return new Tuple(new_terms);
}

#endif