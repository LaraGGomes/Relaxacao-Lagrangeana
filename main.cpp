#include <iostream>
#include "leitor-instancias/src/Data.h"
#include "min-spanning-tree/Kruskal.h"
#include "Solucao.h"

using namespace std;

int main(int argc, char** argv) {

    auto data = Data(2, argv[1]);
    data.read();
    string branching = argv[2];

    size_t n = data.getDimension();

	vvi cost(n, vector<double>(n));
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
            cost[i][j] = data.getDistance(i+1,j+1);
		}
	}

    auto c = Kruskal(cost);

    auto res = c.MST(data.getDimension()-1); // -1 (sem o primeiro vértice)
    
    vii edges = c.getEdges();

    cout << edges.size() << endl;

    MSTpra1Arvore(edges, res, data);

    cout << edges.size() << endl;
    for (auto par : edges) {
        cout << par.first << ' ' << par.second << endl;
    }

    vector<int> g = calcularGraus(edges, data);

    for (int i : g) cout << i << ' ';
    cout << endl;

    cout << "\nCusto original: " << res << endl;

    vector<int> pen{0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // teste
    double soma_q = 0;

    double LB = calcularLB(g, pen, res, soma_q);

    cout << "Custo penalizado: " << LB << endl;
    cout << "Soma dos quadrados = " << soma_q << endl;

    // UB é 148 mesmo?

    return 0;
}
