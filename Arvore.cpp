#include "Arvore.h"

int verticeEscolhido(vector<vector<int>> &listaAdj){
    int maior = 0;
    for (int i = 1; i < listaAdj.size(); i++) {
        if (listaAdj[i].size() >= listaAdj[maior].size()) 
            maior = i;
    }

    return maior;
}

bool ehViavel(vector<vector<int>> &listaAdj) {
    for (int i = 0; i < listaAdj.size(); i++) {
        if (listaAdj[i].size() != 2) return false;
    }

    return true;
}