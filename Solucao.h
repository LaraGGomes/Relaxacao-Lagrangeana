#ifndef SOLUCAO_H
#define SOLUCAO_H

#include <limits>
#include "Arvore.h"
#include "Branching.h"
#include "leitor-instancias/src/Data.h"
#include "min-spanning-tree/Kruskal.h"

using namespace std;

void MSTpra1Arvore(vii &arestas, double &custo, Data &data);
vector<int> calcularGraus(vii &arestas, Data &data);
double calcularLB(const vector<int> &graus, vector<int> &pen, const double &custo, double &soma_quadrados);
bool stopCriterion(const vector<int> &graus, const vector<int> &pen);

vector<int> SolveLagrangianDual(int UB, double puloMin, int kMax, vvi &cost, Data &data);

#endif