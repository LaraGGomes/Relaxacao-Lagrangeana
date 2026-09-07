#ifndef SOLUCAO_H
#define SOLUCAO_H

#include <limits>
#include "Arvore.h"
#include "Branching.h"
#include "leitor-instancias/src/Data.h"
#include "min-spanning-tree/Kruskal.h"

using namespace std;

void MSTpra1Arvore(vii &, double &, Data &);
vector<vector<int>> calcularGraus(vii &, Data &);
double calcularLB(const vector<vector<int>> &, vector<double> &, const double &, double &);
bool stopCriterion(const vector<vector<int>> &, const vector<double> &);

vector<double> SolveLagrangianDual(int, double, int, double &, vii &, vector<double> &, vvi &, Data &);

#endif