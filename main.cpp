#include <iostream>
#include "leitor-instancias/src/Data.h"
#include "min-spanning-tree/Kruskal.h"
#include "Solucao.h"

using namespace std;

int main(int argc, char** argv) {

    auto data = Data(2, argv[1]);
    data.read();
    string branching = argv[2];
    double UB = stod(argv[3]);

    size_t n = data.getDimension();

	vvi cost(n, vector<double>(n));
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
            cost[i][j] = data.getDistance(i+1,j+1);
		}
	}

    double res = framework(UB+1, branching, cost);

    cout << "Melhor custo: " << res << '\n';

    return 0;
}
