#include "Solucao.h"

void novaSolucao(no &node, size_t &n, double **c) {
    int v1, v2;
    vector<double> custosOriginais(node.arcos_proibidos.size());
    
    // proibir todos os arcos
    for (int i = 0; i < node.arcos_proibidos.size(); i++) {
        v1 = node.arcos_proibidos[i].first -1, v2 = node.arcos_proibidos[i].second -1;

        custosOriginais[i] = c[v1][v2];
        c[v1][v2] = 99999999;
    }

    // construindo nova solução
    hungarian_problem_t p;

    hungarian_init(&p, c, n, n, HUNGARIAN_MODE_MINIMIZE_COST);

    node.lower_bound = hungarian_solve(&p);
    
    node.subtour = subtours(&p);
    node.escolhido = subtourEscolhido(node.subtour);
    node.viavel = ehViavel(node.subtour);

    // reinserir todos os custos originais dos arcos proibidos
    for (int i = 0; i < node.arcos_proibidos.size(); i++) {
        v1 = node.arcos_proibidos[i].first -1, v2 = node.arcos_proibidos[i].second -1;

        c[v1][v2] = custosOriginais[i];
    }

    hungarian_free(&p);
}

double framework(string &modo, size_t tam, double **matrizCusto) {
    no raiz;
    raiz.arcos_proibidos = {};
    novaSolucao(raiz, tam, matrizCusto);

    list<no> arvore;
    arvore.push_back(raiz);
    
    double upper_bound = numeric_limits<double>::infinity();

    while (!arvore.empty()) {

        auto node = branchingStrategy(arvore, modo);

        if (node->lower_bound >= upper_bound) {
            arvore.erase(node);
            continue;
        }

        if (node->viavel) {
            upper_bound = min(upper_bound, node->lower_bound);
        }

        else {
            for (int i = 0; i < node->subtour[node->escolhido].size() - 1; i++) {
                no n;
                n.arcos_proibidos = node->arcos_proibidos;

                pair<int,int> arco_proibido = {
                    node->subtour[node->escolhido][i], 
                    node->subtour[node->escolhido][i+1]
                };

                n.arcos_proibidos.push_back(arco_proibido);

                novaSolucao(n, tam, matrizCusto);

                if (n.lower_bound < upper_bound) {
                    arvore.push_back(n);
                }
            }
        }

        arvore.erase(node);
    }

    return upper_bound;
}
/*
    x* = vetor com variáveis binárias de existência das arestas 
    c = custo associado a aresta x
    b - Ax* = 2 - grau(i), sendo i a linha da aresta x
*/

vector<int> SolveLagrangianDual(int UB, double puloMin, int kMax, vvi &cost) {
    vector<int> penalizadores; // inicializar com 0's
    double pulo = 1;
    int k = 0;

    while (pulo >= puloMin) {
        // x* solução Kruskal

        // w
        // com a solução, multiplico todos as arestas (usar o getEdges) pelo seu respectivo custo, 
        // e cálculo do custo associado ao penalizador (com função de retornar grau do vértice)

        // if w > w*
        // else if

        // cálculo tamanho do pulo

        // atualização dos penalizadores

        // critério de parada if (grau(i) >= 2 para todo i && penalizadores*(grau(i) - 2) = 0) ou w >= UB
    }

    // retornar bestPenalizadores
}