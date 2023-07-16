// Uma implementação do algoritmo Highest Push-Relabel para o problema de fluxo máximo.
// Proposto por 
// Tempo de pior caso: O(n^2 * sqrt(m))

// Sobre a versão:
// Adaptamos a forma de arazenamento de arcos do grafo para que possamos
// realizar a operação de discharge de um modo não muito convencional.
// Trocamos as listas de adjascência por um vetor de adjascência para cada nó
// (difere da matriz de adjascência, uma posição no vetor não corresponde ao nó de destino)
// Sempre que realizamos a operação de discharge sobre um nó u, ordenamos o vetor de adjascência de
// Essa ideia visto que o grau média de adjascência por nó eram na média de 5 arcos

#ifndef HIGHEST_PUSH_RELABEL_V2
#define HIGHEST_PUSH_RELABEL_V2

#include <queue>
#include <vector>

#include "graph.h"
#include "insertionSort.h"

class highest_push_relabel_max_flow_v2 {
  GraphAdjacencyVector Gf;
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
  void pushFlow(int u, vectorList::edge *e, int flow);
  void relabel(int u, vectorList::edge e);
  void discharge(int u);

public:
  // ctor
  highest_push_relabel_max_flow_v2(GraphAdjacencyVector &G);

  int solve(int s, int t);

  int get_max_flow() { return max_flow; };
  long get_num_pushs() { return num_pushs; };
  long get_num_relabels() { return num_relabels; };
  long get_time_execution() { return time_execution; };
};

#endif