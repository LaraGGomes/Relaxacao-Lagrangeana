#ifndef ARVORE_H
#define ARVORE_H

#include <vector>
#include <algorithm>
#include <list>
#include "min-spanning-tree/Kruskal.h"

using namespace std;

typedef struct noArvore {
    vii arcos_proibidos;
    vector<vector<int>> listaAdj;
    vector<double> penalizadores;
    double lower_bound; // custo total da solução
    int escolhido;
    bool viavel;
}no;

int verticeEscolhido(vector<vector<int>> &);
bool ehViavel(vector<vector<int>> &);

#endif