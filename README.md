# Branch and Bound (BnB)

Compilar o programa: 

    g++ *.cpp algoritmo-hungaro/src/Data.cpp algoritmo-hungaro/src/hungarian.cpp -o programa.exe

    g++ -O3 *.cpp min-spanning-tree/Kruskal.cpp leitor-instancias/src/Data.cpp -o programa.exe

Rodar uma instância:

    ./programa.exe instances/nomedainstancia.tsp branchingStrategy

No campo de estratégia de branching, digite entre as opções: DFS (Depth-First Search), BFS (Breadth-First Search) ou LB (menor lower bound)
A estratégia base é DFS.

