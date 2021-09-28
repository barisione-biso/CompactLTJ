#ifndef UTILS_H
#define UTILS_H

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

#endif