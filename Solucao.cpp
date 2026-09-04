#include "Solucao.h"

void MSTpra1Arvore(vii &arestas, double &custo, Data &data) {
    int n = data.getDimension();
    pair<double,int> v1 = {99999999, 1}, v2 = {99999999, 1};

    for (int i = 2; i <= n; i++) {
        double c = data.getDistance(1, i);

        if (c < v1.first) {
            v2 = {v1.first, v1.second};
            v1 = {c, i};
        }
        else if (c < v2.first) {
            v2 = {c, i};
        }
    }

    custo = custo + v1.first + v1.second;   // já adiantando a somatória do custo total (cx)

    arestas.push_back({0, v1.second-1});
    arestas.push_back({0, v2.second-1});
}

vector<int> calcularGraus(vii &arestas, Data &data) {
    vector<int> graus(data.getDimension(), 0);

    for (const auto &[i, j] : arestas) {
        graus[i]++;
        graus[j]++;
    }

    return graus;
}

double calcularLB(const vector<int> &graus, vector<int> &pen, const double &custo, double &soma_quadrados) {
    double penalidadeTotal = 0;

    for (int i = 0; i < pen.size(); i++) {
        double violacao = 2 - graus[i];
        penalidadeTotal += pen[i] * violacao;

        soma_quadrados += violacao*violacao;    // pra att os penalizadores depois
    }

    return custo + penalidadeTotal;
}

bool stopCriterion(const vector<int> &graus, const vector<int> &pen) {
    for (int i = 0; i < pen.size(); i++) {
        if (graus[i] < 2) return false;

        if (pen[i]*graus[i] != 0) return false;
    }

    return true;
}

/*
    x* = vetor com variáveis binárias de existência das arestas 
    c = custo associado a aresta x
    b - Ax* = 2 - grau(i), sendo i a linha da aresta x
*/

vector<int> SolveLagrangianDual(int UB, double puloMin, int kMax, vvi &cost, Data &data) {
    int n = data.getDimension();
    vector<int> penalizadores(n, 0);
    vector<int> bestPenalizadores;
    double pulo = 1;
    int k = 0;
    double bestW = 0;

    while (pulo >= puloMin) {
        // x* solução Kruskal
        auto x = Kruskal(cost);

        double xCusto = x.MST(n-1);
        vii x_edges = x.getEdges();
        
        MSTpra1Arvore(x_edges, xCusto, data);

        double soma_quadrados = 0;
        vector<int> graus = calcularGraus(x_edges, data);
        double w = calcularLB(graus, penalizadores, xCusto, soma_quadrados);

        if (w > bestW) {
            bestW = w;
            bestPenalizadores = penalizadores;
            k = 0;
        }
        else {
            k++;
            if (k >= kMax) {
                k = 0;
                pulo /= 2.0;
            }
        }

        // cálculo tamanho do pulo
        double tp = pulo*(UB - w)/soma_quadrados;

        // atualização dos penalizadores
        for (int i = 0; i < n; i++) {
            penalizadores[i] += tp*(2 - graus[i]);
        }

        // critério de parada
        if (w >= UB || stopCriterion(graus, penalizadores)) break;
    }

    return bestPenalizadores;

    // teoricamente é isso, testar direitinho depois, principalmente os cálculos
}