// Uma implementação do algoritmo Highest Push-Relabel para o problema de fluxo máximo.
// Proposto por Ahuja e Orlin aprimorado por Cheriyan e Mehlhorn
// Tempo de pior caso: O(n^2 * sqrt(m))

// Sobre a versão 5:
// Nesta versão a definição de nó ativo deixa de ser exclusivamente relacionado ao excesso
// de um nó. Agora um nó é ativo se ef(i) > 0 e h(i) >= n para todo i /in V - {s, t}
// Nós com altura maior que n não conseguem alcançar o terminal, e portanto não podem
// contribuir para o aumento de fluxo recebido por ele.
//
// Além disso, acrescentamos o método gap que identifica a criação de uma lacuna de altura 
// e realiza um relabel em nós acima desse gap para evitar realizar operações em nós
// que poderiam criar um ciclho de pushs e relabels entre si, até chegar na altura n

#ifndef HIGHEST_PUSH_RELABEL_V6
#define HIGHEST_PUSH_RELABEL_V6

#include "graph.h"
#include "list_node_height.h"

#include <vector>
#include <queue>


class highest_push_relabel_max_flow_v6 {
  GraphAdjListWitchReverse Gf;
  int src, sink;
  int num_nodes;

  int d = 0;
  long max_flow = 0;
  long num_pushs = 0;
  long num_relabels = 0;
  long num_gaps = 0;
  long time_execution = 0;
  
  std::vector<int> node_excess;
  std::vector<int> node_height;
  std::vector<std::queue<int>> bucket;
  list_node_height list_height;

  void initializePreflow();
  void pushFlow(int u, LinkedListWithReverse::edge *e, int flow);
  void relabel(int u);
  bool isAdmissible(int u, LinkedListWithReverse::edge *e);
  void discharge(int u);
  void gap(int k);

public:
  // ctor
  highest_push_relabel_max_flow_v6(GraphAdjListWitchReverse &G);

  int solve(int s, int t);

  int get_max_flow() { return max_flow; };
  long get_num_pushs() { return num_pushs; };
  long get_num_relabels() { return num_relabels; };
  long get_num_gaps() { return num_gaps; };
  long get_time_execution() { return time_execution; };
};

#endif