#ifndef COMPACT_TRIE_H
#define COMPACT_TRIE_H

#include <sdsl/vectors.hpp>
#include <vector>
#include <iostream>

using namespace std;
using namespace sdsl;

typedef unsigned int u_int;
 
class CompactTrie{

    private:
        u_int key;
        bool atEnd;
        bit_vector B;
        vector<u_int> S;
        rank_support_v<1> b_rank1; 
        rank_support_v<0> b_rank0;
        select_support_mcl<0> b_sel0;
        select_support_mcl<1> b_sel1;

    public:
        // CompactTrie(bit_vector B, vector<u_int> S);
        CompactTrie(int val){
            cout<<"el val es "<<val<<endl;
        }

        CompactTrie(bit_vector b, vector<u_int> s){
            B = b;
            S = s;
            util::init_support(b_rank1,&B);
            // rank_support_v<1> b_rank1(&B); 
        // rank_support_v<0> b_rank0(&B);
        // select_support_mcl<0> b_sel0(&B);
        // select_support_mcl<1> b_sel1(&B);
        }

        void access_S(int i){
            cout<<S[i]<<endl;
        }


};

// class Date
// {
// private:
//     int m_year;
//     int m_month;
//     int m_day;
 
// public:
//     Date(int year, int month, int day);
 
//     void SetDate(int year, int month, int day);
 
//     int getYear() { return m_year; }
//     int getMonth() { return m_month; }
//     int getDay()  { return m_day; }
// };
 
#endif