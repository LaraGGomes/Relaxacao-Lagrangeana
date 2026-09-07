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

vector<vector<int>> calcularGraus(vii &arestas, Data &data) {
    vector<vector<int>> listaAdj(data.getDimension());

    for (const auto &[i, j] : arestas) {
        listaAdj[i].push_back(j);
        listaAdj[j].push_back(i);
    }

    return listaAdj;
}

double calcularLB(const vector<vector<int>> &graus, vector<double> &pen, const double &custo, double &soma_quadrados) {
    double penalidadeTotal = 0;

    for (int i = 0; i < pen.size(); i++) {
        double violacao = 2 - graus[i].size();
        penalidadeTotal += pen[i] * violacao;

        soma_quadrados += violacao*violacao;    // pra att os penalizadores depois
    }

    return custo + penalidadeTotal;
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

vector<double> SolveLagrangianDual(double UB, double puloMin, int kMax, double &bestW, vii &bestEdges, vector<double> &pen, vvi &cost, Data &data) {
    int n = data.getDimension();
    vector<double> penalizadores(n, 0);
    vector<double> bestPenalizadores;
    double pulo = 1;
    int k = 0;
    bestW = 0;

    int var = 3;

    while (pulo >= puloMin) {
        // x* solução Kruskal
        auto x = Kruskal(cost, penalizadores);

        
        double xCusto = x.MST(n-1);
        vii x_edges = x.getEdges();

        //cout << "Kruskal " << xCusto << '\n';
        
        MSTpra1Arvore(x_edges, xCusto, data);

        // if (var) {
        //     cout << "Penalizadores: " << endl;
        //     for (auto x : penalizadores) {
        //         cout << x << ' ';
        //     }
        //     cout << "\n";
        //     var--;
        // }

        double soma_quadrados = 0;
        vector<vector<int>> graus = calcularGraus(x_edges, data);
        double w = calcularLB(graus, penalizadores, xCusto, soma_quadrados);

        // cout << "Custo penalizado " << w << "\n\n";

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

        // cálculo tamanho do pulo
        double tp = pulo*(UB - w)/soma_quadrados;

        // atualização dos penalizadores
        for (int i = 0; i < n; i++) {
            penalizadores[i] += tp*(2 - graus[i].size());
        }

        // critério de parada
        if (w >= UB || stopCriterion(graus, penalizadores)) break;
    }

    return bestPenalizadores;

    // depois checar se as mudanças feitas no Kruskal (adicionei os 
    // penalizadores ao custo) estão corretas
}

void novaSolucao(no &node, double UB, double puloMin, int kMax, vector<double> &pen, vvi &cost, Data &data) {
    int v1, v2;
    vector<double> custosOriginais(node.arcos_proibidos.size());

    // proibir os arcos
    for (int i = 0; i < node.arcos_proibidos.size(); i++) {
        v1 = node.arcos_proibidos[i].first +1, v2 = node.arcos_proibidos[i].second +1;

        custosOriginais[i] = cost[v1][v2];
        cost[v1][v2] = 99999999;
    }

    // algoritmo
    double LB;
    vii arestas;
    vector<double> p = SolveLagrangianDual(UB, puloMin, kMax, LB, arestas, pen, cost, data);

    node.lower_bound = LB;
    node.penalizadores = p;

    // verificação se é um tour (viável) e escolher vértice para proibir vértices
    vector<vector<int>> listaAdj = calcularGraus(arestas, data);
    node.listaAdj = listaAdj;

    node.viavel = ehViavel(listaAdj);
    if (!node.viavel) {
        node.escolhido = verticeEscolhido(listaAdj);
    }

    // reinserir custos originais
    for (int i = 0; i < node.arcos_proibidos.size(); i++) {
        v1 = node.arcos_proibidos[i].first -1, v2 = node.arcos_proibidos[i].second -1;

        cost[v1][v2] = custosOriginais[i];
    }
}

double framework(string &modo, vvi &cost, Data &data) {
    no raiz;
    raiz.arcos_proibidos = {};
    vector<double> penalizadores(data.getDimension(), 0);
    double puloMin = 10e-5;
    int kMax = 30;

    double LB;
    double UB = numeric_limits<double>::infinity();

    novaSolucao(raiz, UB, puloMin, kMax, penalizadores, cost, data);

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

                novaSolucao(n, UB, puloMin, kMax, node->penalizadores, cost, data);

                if (n.lower_bound < UB) {
                    arvore.push_back(n);
                }
            }
        }
    }

    return UB;
}