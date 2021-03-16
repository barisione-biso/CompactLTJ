#include <iostream>
#include <sdsl/vectors.hpp>
#include <sdsl/rank_support.hpp>
#include <sdsl/select_support.hpp>
#include "compact_trie.hpp" 

using namespace std;
using namespace sdsl;

typedef unsigned int u_int;
int main()
{
    // int_vector<> v = {3,2,1,0,2,1,3,4,1,1,1,3,2,3};
    // v[1] = 0;
    // util::bit_compress(v);
    // cout << v << endl;
    // cout << size_in_bytes(v) << endl;
    // cout<<"HOLA"<<endl;

    bit_vector B = {1,1,0,1,1,0,1,1,1,0,1,1,0,0,0,1,1,1,0,0,0,0,1,0,0,1,0,1,0,0};
    //              0 1 2 3 4 5 6 7 8 9
    vector<u_int> S = {0, 0, 1, 3, 3, 4, 5, 4, 5, 6, 8, 9, 2, 5, 2};

    cout<<"hola"<<endl;
    CompactTrie ct = CompactTrie(B,S);
    // ct.access_S(2);
    // u_int it = 3;
    // rank_support_v<1> b_rank1(&B); 
    // rank_support_v<0> b_rank0(&B);
    // select_support_mcl<0> b_sel0(&B);
    // select_support_mcl<1> b_sel1(&B);

    // //key()
    // u_int key = S[b_rank1(it+1)-1];
    // cout<<"La key es "<<key<<endl;

    // //next()
    // if(it<B.size()-1 && !B[it+1]){
    //     cout<<"Tiene next"<<endl;
    //     it++;
    // }
    // else{
    //     cout<<"No tienen next"<<endl;
    //     //at end = true
    // }

    // //seek(int seekKey)
    // u_int seekKey = 5;
    // u_int cant_0 = b_rank0(it+1);
    // u_int prox_0 = b_sel0(cant_0 + 1);

    // u_int start_index = b_rank1(it+1) - 1;
    // u_int end_index = b_rank1(prox_0) - 1; //prox_0 - 1 por que el it solo puede llegar hasta ahí, pero +1 por forma del rank

    // bool change = false;
    // for(int i=start_index; i<=end_index; i++){
    //     cout<<"el i es "<<i<<endl;
    //     if(S[i]>=seekKey){
    //         u_int key = S[b_rank1(i+1)-1];
    //         it = b_sel1(i+1);
    //         cout<<"it pasa a ser "<<it<<" con key "<<key<<endl;
    //         change = true;
    //     }
    // }

    // if(!change){
    //     cout<<"No hay seekkey, paso al final"<<endl;
    //     //at end
    // }

    //open
    // u_int cant_0_0 = b_rank0(it);
    // it = b_sel0(cant_0_0 + 1) + 1;
    // cout<<"el nuevo it es "<<it<<endl;

    // //up
    // // 1. asegurarnos que no estamos en raiz
    // // 2. calcular padre
    // //vamos al 0 previo al nivel actual
    // u_int cant_0 = b_rank0(it);
    // u_int prev_0 = b_sel0(cant_0);

    // //vamos al parentesis que cierra este, el cual pertenecerá al nivel del padre
    // u_int cant_1 = b_rank1(prev_0);
    // u_int prev_1 = b_sel1(cant_1);

    // //vamos al 0 previo al nivel del padre y luego al padre
    // u_int cant_0_0 = b_rank0(prev_1);
    // u_int prev_0_0 = b_sel0(cant_0_0);

    // u_int padre = prev_0_0 + 1;





}