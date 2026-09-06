#ifndef ARVORE_H
#define ARVORE_H

#include <vector>
#include <algorithm>
#include <list>
#include "algoritmo-hungaro/src/hungarian.h"
#include "min-spanning-tree/Kruskal.h"

using namespace std;

typedef struct noArvore {
    vii arcos_proibidos;
    vector<vector<int>> subtour;    // n vai mais usar
    vector<double> penalizadores;
    double lower_bound; // custo total da solução
    int escolhido;
    bool viavel;
}no;

vector<pair<int,int>> arcos_proibidos(vector<vector<int>> &subtours, int escolhido);
vector<vector<int>> subtours(hungarian_problem_t* p);
int subtourEscolhido(vector<vector<int>> &subtours);
bool ehViavel(vector<vector<int>> &subtours);

#endif