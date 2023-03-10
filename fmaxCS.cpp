#include "bfs.cpp"
#include "graph.cpp"
#include "math.h"
#include <algorithm>

int maxFlowCS(Graph &G, int s, int t, int *numPaths) {

  int u, v, max_capacity = 0, flow = 0;
  int parent[G.getNumVertices() + 1];
  Graph Gf(G);

  // Obtém a maior capacidade dentre os arcos de G
  for (u = 1; u <= G.getNumVertices(); u++) {
    Edge *aux = G.getEdge(u);

    while (aux != NULL) {
      max_capacity = max(max_capacity, aux->capacity);
      aux = aux->next;
    }
  }

  int delta = pow(2, int(log(max_capacity) / log(2)));

  while (delta >= 1) {

    cout << delta << endl;

    auto initGrafo = chrono::steady_clock::now();
    Graph Gf_delta(Gf, delta);
    auto endGrafo = chrono::steady_clock::now();

    auto elapsed = endGrafo - initGrafo;
    cout << "tempo construcao Gf(delta): " << elapsed.count() << endl;

    auto initLoop = chrono::steady_clock::now();

    // Empurra fluxo enquanto existir um (s,t)-caminho no Grafo residual Delta
    while (bfs(Gf_delta, s, t, parent)) {

      int path_capacity = INT_MAX;

      // Obtem a capacidade residual mínima do caminho encontrado
      for (v = t; v != s; v = parent[v]) {
        u = parent[v];
        path_capacity = min(path_capacity, Gf.getCapacityEdge(u, v));
      }

      // Atualiza as capacidades residuais dos arcos de Gf_delta e Gf
      for (v = t; v != s; v = parent[v]) {
        u = parent[v];
        
        Gf_delta.addCapacityEdge(u, v, -path_capacity);
        Gf_delta.addCapacityEdge(v, u, +path_capacity);

        Gf.addCapacityEdge(u, v, -path_capacity);
        Gf.addCapacityEdge(v, u, +path_capacity);
      }

      flow += path_capacity;
      *numPaths += 1;
    }

    auto endLoop = chrono::steady_clock::now();
    elapsed = endLoop - initLoop;
    cout << "tempo bfs: " << elapsed.count() << endl;

    //Gf_delta.~Graph();
    delta = int(delta / 2);
  }

  return flow;
}