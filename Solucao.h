#ifndef SOLUCAO_H
#define SOLUCAO_H

#include <limits>
#include "Arvore.h"
#include "Branching.h"
#include "leitor-instancias/src/Data.h"
#include "min-spanning-tree/Kruskal.h"

using namespace std;

void MSTpra1Arvore(vii &, double &, Data &);
vector<int> calcularGraus(vii &, Data &);
double calcularLB(const vector<int> &, vector<double> &, const double &, double &);
bool stopCriterion(const vector<int> &, const vector<double> &);

vector<double> SolveLagrangianDual(int, double, int, double &, vector<double> &, vvi &, Data &);

#endif