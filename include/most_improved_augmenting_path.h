// Uma implementação do algoritmo de caminho aumentador mais curto
// Proposto por Edmonds-Karp
// Tempo de pior caso: O(m * log(mU) * (m + n log(n)))

// Ideia geral: Enquanto existir, procura um (s-t)-caminho simples cuja capacidade
// residual mínima do caminho seja a maior possível e empurra fluxo ao longo dele

#ifndef LARGER_AUGMENTING_PATH
#define LARGER_AUGMENTING_PATH

#include "graph.h"

int most_improved_augmenting_path_max_flow(GraphAdjacencyList &Gf, int s, int t, int *numPaths);

#endif