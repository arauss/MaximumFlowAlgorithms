// Uma implementação do algoritmo Highest Push-Relabel para o problema de fluxo máximo.
// Proposto por Goldberg e Tarjan
// Tempo de pior caso: O(n^2 * sqrt(m))

// Sobre a versão:
// Incluímos a operação de discharge que sempre realiza pushs e relabels sobre
// o nó ativo com maior rótulo de altura até que seu excesso seja zerado.

#ifndef PUSH_RELABEL
#define PUSH_RELABEL

#include "graph.h"
#include "heapNode.h"

#include <vector>

class push_relabel_max_flow {
  GraphAdjacencyList Gf;
  BinaryHeap pq;
  int src, sink;
  int num_nodes;

  int max_flow = 0;
  long num_pushs = 0;
  long num_relabels = 0;
  long time_execution = 0;
  
  std::vector<int> node_excess;
  std::vector<int> node_height;

  void initializePreflow();
  void pushFlow(int u, LinkedList::edge *e, int flow);
  void relabel(int u);
  LinkedList::edge *existAdmissibleEdge(int u);

public:
  // ctor
  push_relabel_max_flow(GraphAdjacencyList &G);

  int solve(int s, int t);

  int get_max_flow() { return max_flow; };
  long get_num_pushs() { return num_pushs; };
  long get_num_relabels() { return num_relabels; };
  long get_time_execution() { return time_execution; };
};

#endif