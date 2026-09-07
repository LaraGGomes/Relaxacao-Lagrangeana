#ifndef BRANCHING_H
#define BRANCHING_H

#include "Arvore.h"
#include <string>
#include <queue>

list<no>::iterator branchingStrategy(list<no> &, string &);
list<no>::iterator DFS(list<no> &);
list<no>::iterator BFS(list<no> &);
list<no>::iterator menorLB(list<no> &);

#endif