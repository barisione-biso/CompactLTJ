#ifndef WAVELET_TREE_H
#define WAVELET_TREE_H

#include<iostream>
#include <sdsl/wavelet_trees.hpp>


using namespace std;
using namespace sdsl;

class WaveletTree{

    private:

        wt_int<> wt;

    public:

        WaveletTree(int x){
            cout<<"hola"<<endl;
        }

        WaveletTree(){
            construct_im(wt, "1 3 3 4 5 4 5 6 8 9 2 5 2", 'd');
            cout<<"construido"<<endl;
            for(int i=0; i<wt.size(); i++){
                cout<<wt[i]<<endl;
            }
            
        }

};

#endif