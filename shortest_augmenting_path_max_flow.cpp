#include "graph.cpp"
#include "./dependencies/bfs.cpp"

#include <algorithm>
#include <limits>

/* Algoritmo de Edmonds-Karp: caminho aumentador mais curto */

int shortest_augmenting_path_max_flow(ListGraph &G, int s, int t, int *numPaths) {
  
  int u, v, flow = 0;
  int parent[G.num_vertices()];
  ListGraph Gf(G);

  // Empurra fluxo enquanto existir um (s,t)-caminho
  while (bfs(Gf, s, t, parent)) {

    // Encontra a capacidade residual mínima do caminho encontrado
    int path_capacity = INT_MAX;

    for (v = t; v != s; v = parent[v]) {
      u = parent[v];
      path_capacity = min(path_capacity, Gf.get_capacity(u, v));
    }

    // atualiza os fluxos no grafo original e as capacidades residuais dos arcos no caminho encontrado
    for (v = t; v != s; v = parent[v]) {
      u = parent[v];

      G.add_flow(u, v, +path_capacity);

      Gf.add_capacity(u, v, -path_capacity);  // arco direto
      Gf.add_capacity(v, u, +path_capacity);  // arco reverso
    }

    flow += path_capacity;
    *numPaths += 1;
  }
  
  return flow;
}