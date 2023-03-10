#include "bfs.cpp"
#include "graph.cpp"
#include <algorithm>
#include <limits>
#include <chrono>

int maxFlowSAP(Graph &G, int s, int t, int *numPaths) {
  
  int u, v, flow = 0;
  int parent[G.getNumVertices()];
  Graph Gf(G);

  // Empurra fluxo enquanto existir um (s,t)-caminho
  while (bfs(Gf, s, t, parent)) {

    // Encontra a capacidade residual mínima do caminho encontrado
    int path_capacity = INT_MAX;

    for (v = t; v != s; v = parent[v]) {
      u = parent[v];
      path_capacity = min(path_capacity, Gf.getCapacityEdge(u, v));
    }

    // atualiza os fluxos no grafo original e as capacidades residuais dos arcos no caminho encontrado
    for (v = t; v != s; v = parent[v]) {
      u = parent[v];

      G.addFlowEdge(u, v, +path_capacity);

      Gf.addCapacityEdge(u, v, -path_capacity);  // arco direto
      Gf.addCapacityEdge(v, u, +path_capacity);  // arco reverso
    }

    flow += path_capacity;
    *numPaths += 1;
  }
  
  return flow;
}