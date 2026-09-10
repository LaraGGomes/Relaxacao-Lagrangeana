#ifndef SOLUCAO_H
#define SOLUCAO_H

#include <limits>
#include "Branching.h"
#include "min-spanning-tree/Kruskal.h"

using namespace std;

void MSTpra1Arvore(vii &, double &, vector<double> &, vvi &);
vector<vector<int>> calcularGraus(vii &, int);
bool stopCriterion(const vector<vector<int>> &, const vector<double> &);

vector<double> SolveLagrangianDual(double, double, int, double &, vii &, vector<double> &, vvi &);
void novaSolucao(no &, double, double, int, vector<double> &, vvi &);
double framework(double, string &, vvi &);

#endif