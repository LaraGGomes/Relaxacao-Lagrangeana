#include <iostream>
#include "leitor-instancias/src/Data.h"
#include "min-spanning-tree/Kruskal.h"
//#include "Solucao.h"

using namespace std;

int main(int argc, char** argv) {

    auto data = Data(2, argv[1]);
    data.read();
    string branching = argv[2];

    size_t n = data.getDimension();

	vvi cost(n-1, vector<double>(n-1)); // vetor distancias mod (sem o primeiro, vulgo vértice 1)
	for (int i = 0; i < n-1; i++){
		for (int j = 0; j < n-1; j++){
            cost[i][j] = data.getDistance(i+2,j+2);
		}
	}

    auto c = Kruskal(cost);

    auto res = c.MST(data.getDimension()-1);
    
    vii edges = c.getEdges();

    cout << edges.size() << endl;
    for (auto par : edges) {
        cout << par.first << ' ' << par.second << endl;
    }

    return 0;
}
