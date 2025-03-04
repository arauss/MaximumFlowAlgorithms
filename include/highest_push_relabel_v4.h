// Uma implementação do algoritmo Highest Push-Relabel para o problema de fluxo máximo.
// Proposto por 
// Tempo de pior caso: O(n^2 * sqrt(m))

// Sobre a versão 4:
// Nesta versão a definição de nó ativo deixa de ser exclusivamente relacionado ao excesso
// de um nó. Agora um nó é ativo se ef(i) > 0 e h(i) >= n para todo i /in V - {s, t}
// Nós com altura maior que n não conseguem alncaçar o terminal, e portanto não podem
// contribuir para o aumento de fluxo recebido por ele.

#ifndef HIGHEST_PUSH_RELABEL_V4
#define HIGHEST_PUSH_RELABEL_V4

#include "graph.h"

#include <vector>
#include <queue>


class highest_push_relabel_max_flow_v4 {
  GraphAdjacencyList Gf;
  int src, sink;
  int num_nodes;

  int d = 0;
  int max_flow = 0;
  long num_pushs = 0;
  long num_relabels = 0;
  long time_execution = 0;
  
  std::vector<int> node_excess;
  std::vector<int> node_height;
  std::vector<std::queue<int>> bucket;

  void initializePreflow();
  void pushFlow(int u, LinkedList::edge *e, int flow);
  void relabel(int u);
  bool isAdmissible(int u, LinkedList::edge *e);
  void discharge(int u);

public:
  // ctor
  highest_push_relabel_max_flow_v4(GraphAdjacencyList &G);

  int solve(int s, int t);

  int get_max_flow() { return max_flow; };
  long get_num_pushs() { return num_pushs; };
  long get_num_relabels() { return num_relabels; };
  long get_time_execution() { return time_execution; };
};

#endif