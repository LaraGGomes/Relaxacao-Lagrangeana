#include "Solucao.h"

void MSTpra1Arvore(vii &arestas, double &custo, vector<double> &pen, vvi &cost) {
    int n = cost.size();
    pair<double,int> v1 = {99999999, 0}, v2 = {99999999, 0};

    for (int i = 1; i < n; i++) {
        double c = cost[0][i] - pen[i];

        if (c < v1.first) {
            v2 = {v1.first, v1.second};
            v1 = {c, i};
        }
        else if (c < v2.first) {
            v2 = {c, i};
        }
    }

    custo = custo + v1.first + v2.first;   // já adiantando a somatória do custo total (cx)

    arestas.push_back({0, v1.second});
    arestas.push_back({0, v2.second});
}

vector<vector<int>> calcularGraus(vii &arestas, int tam) {
    vector<vector<int>> listaAdj(tam);

    for (const auto &[i, j] : arestas) {
        listaAdj[i].push_back(j);
        listaAdj[j].push_back(i);
    }

    return listaAdj;
}

bool stopCriterion(const vector<vector<int>> &graus, const vector<double> &pen) {
    for (int i = 0; i < pen.size(); i++) {
        if (graus[i].size() < 2) return false;

        if (pen[i]*graus[i].size() != 0) return false;
    }

    return true;
}

/*
    x* = vetor com variáveis binárias de existência das arestas 
    c = custo associado a aresta x
    b - Ax* = 2 - grau(i), sendo i a linha da aresta x
*/

vector<double> SolveLagrangianDual(double UB, double puloMin, int kMax, double &bestW, vii &bestEdges, vector<double> &pen, vvi &cost) {
    int n = cost.size();
    vector<double> penalizadores(n, 0);
    vector<double> bestPenalizadores;
    double pulo = 1;
    int k = 0;
    bestW = 0;

    while (pulo >= puloMin) {
        // x* solução Kruskal
        auto x = Kruskal(cost, penalizadores);

        double xCusto = x.MST(n);
        vii x_edges = x.getEdges();
        
        MSTpra1Arvore(x_edges, xCusto, penalizadores, cost);

        vector<vector<int>> graus = calcularGraus(x_edges, cost.size());
        
        double soma_quadrados = 0;

        double w = xCusto; 
        for(int i = 0; i < n; i++) {
            int violacao = 2 - graus[i].size();
            soma_quadrados += violacao*violacao;

            w += 2.0 * penalizadores[i]; 
        }

        if (w > bestW) {
            bestW = w;
            bestPenalizadores = penalizadores;
            bestEdges = x_edges;
            k = 0;
        }
        else {
            k++;
            if (k >= kMax) {
                k = 0;
                pulo /= 2.0;
            }
        }

        if (w >= UB || stopCriterion(graus, penalizadores)) break;

        // cálculo tamanho do pulo
        double tp = pulo*(UB - w)/soma_quadrados;

        // atualização dos penalizadores
        for (int i = 1; i < n; i++) {
            penalizadores[i] += tp*(2.0 - graus[i].size());
        }
    }

    return bestPenalizadores;
}

void novaSolucao(no &node, double UB, double puloMin, int kMax, vector<double> &pen, vvi &cost) {
    int v1, v2;
    vector<double> custosOriginais(node.arcos_proibidos.size());

    // proibir os arcos
    for (int i = 0; i < node.arcos_proibidos.size(); i++) {
        v1 = node.arcos_proibidos[i].first, v2 = node.arcos_proibidos[i].second;

        custosOriginais[i] = cost[v1][v2];
        cost[v1][v2] = 99999999;
        cost[v2][v1] = 99999999;
    }
    
    // algoritmo
    double LB;
    vii arestas;
    vector<double> p = SolveLagrangianDual(UB, puloMin, kMax, LB, arestas, pen, cost);
    
    node.lower_bound = LB;
    node.penalizadores = p;
    
    // verificação se é um tour (viável) e escolher vértice para proibir vértices
    vector<vector<int>> listaAdj = calcularGraus(arestas, cost.size());
    node.listaAdj = listaAdj;
    
    node.viavel = ehViavel(listaAdj);
    if (!node.viavel) {
        node.escolhido = verticeEscolhido(listaAdj);
    }
    
    // reinserir custos originais
    for (int i = 0; i < node.arcos_proibidos.size(); i++) {
        v1 = node.arcos_proibidos[i].first, v2 = node.arcos_proibidos[i].second;
        
        cost[v1][v2] = custosOriginais[i];
        cost[v2][v1] = custosOriginais[i];
    }
}

double framework(double UB, string &modo, vvi &cost) {
    no raiz;
    raiz.arcos_proibidos = {};
    vector<double> penalizadores(cost.size(), 0);
    double puloMin = 10e-5;
    int kMax = 30;

    double LB;

    novaSolucao(raiz, UB, puloMin, kMax, penalizadores, cost);

    list<no> arvore;
    arvore.push_back(raiz);

    while (!arvore.empty()) {
        auto node = branchingStrategy(arvore, modo);

        if (node->viavel) {
            UB = min(UB, node->lower_bound);
        } else {
            // para cada aresta do vértice escolhido, proibir e criar nó filho

            for (int i = 0; i < node->listaAdj[node->escolhido].size(); i++) {
                no n;
                n.arcos_proibidos = node->arcos_proibidos;

                pair<int,int> arcos_proibido = {
                    node->escolhido,
                    node->listaAdj[node->escolhido][i]
                };

                n.arcos_proibidos.push_back(arcos_proibido);

                novaSolucao(n, UB, puloMin, kMax, node->penalizadores, cost);

                if (n.lower_bound <= UB) {
                    arvore.push_back(n);
                }
            }
        }

        arvore.erase(node);
    }

    return UB;
}