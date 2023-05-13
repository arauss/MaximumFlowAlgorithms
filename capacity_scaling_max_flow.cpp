#include "graph.cpp"
#include "./dependencies/bfs.cpp"

#include <math.h>
#include <algorithm>

/* Algoritmo de Ahuja e Orlin: Capacity Scaling */

int capacity_scaling_max_flow(ListGraph &G, int s, int t, int *numPaths) {

  int u, v, max_capacity = 0, flow = 0;
  int parent[G.num_vertices() + 1];
  ListGraph Gf(G);

  // Obtém a maior capacidade dentre os arcos de G
  for (u = 1; u <= G.num_vertices(); u++) {
    LinkedListImplementation::edge *aux = G.get_edge(u);

    while (aux != NULL) {
      max_capacity = max(max_capacity, aux->capacity);
      aux = aux->next;
    }
  }

  int delta = pow(2, int(log(max_capacity) / log(2)));

  while (delta >= 1) {

    ListGraph Gf_delta(Gf, delta);
  
    // Empurra fluxo enquanto existir um (s,t)-caminho no Grafo residual Delta
    while (bfs(Gf_delta, s, t, parent)) {

      int path_capacity = INT_MAX;

      // Obtem a capacidade residual mínima do caminho encontrado
      for (v = t; v != s; v = parent[v]) {
        u = parent[v];
        path_capacity = min(path_capacity, Gf.get_capacity(u, v));
      }

      // Atualiza as capacidades residuais dos arcos de Gf_delta e Gf
      for (v = t; v != s; v = parent[v]) {
        u = parent[v];
        
        Gf_delta.add_capacity(u, v, -path_capacity);
        Gf_delta.add_capacity(v, u, +path_capacity);

        Gf.add_capacity(u, v, -path_capacity);
        Gf.add_capacity(v, u, +path_capacity);
      }

      flow += path_capacity;
      *numPaths += 1;
    }

    delta = int(delta / 2);
  }

  return flow;
}