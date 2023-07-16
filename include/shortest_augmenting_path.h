// Uma implementação do algoritmo de caminho aumentador mais curto
// Proposto por Edmonds-Karp
// Tempo de pior caso: O(m^2 * n)

// Ideia geral: Enquanto existir, procura um (s-t)-caminho simples mais curto
// (menor quantidade de arcos) e empurra fluxo ao longo desse caminho

#ifndef SHORTEST_AUGMENTING_PATH
#define SHORTEST_AUGMENTING_PATH

#include "graph.h"
#include "bfs.h"

int shortest_augmenting_path_max_flow(GraphAdjacencyList &Gf, int s, int t, int *numPaths);

#endif