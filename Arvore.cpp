#include "Arvore.h"

int verticeEscolhido(vector<vector<int>> &graus){
    int maior = 0;
    for (int i = 1; i < graus.size(); i++) {
        if (graus[i].size() > graus[maior].size()) 
            maior = i;
    }

    return maior;
}

bool ehViavel(vector<vector<int>> &graus) {
    for (int i = 0; i < graus.size(); i++) {
        if (graus[i].size() > 2) return false;
    }

    return true;
}