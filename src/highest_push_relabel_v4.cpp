// Uma implementação do algoritmo Highest Push-Relabel para o problema de fluxo máximo.
// Proposto por 
// Tempo de pior caso: O(n^2 * sqrt(m))

// Sobre a versão 4:
// Nesta versão a definição de nó ativo deixa de ser exclusivamente relacionado ao excesso
// de um nó. Agora um nó é ativo se ef(i) > 0 e h(i) >= n para todo i /in V - {s, t}
// Nós com altura maior que n não conseguem alncaçar o terminal, e portanto não podem
// contribuir para o aumento de fluxo recebido por ele.


#include "graph.h"
#include "highest_push_relabel_v4.h"

#include <vector>
#include <queue>
#include <algorithm>
#include <chrono>


highest_push_relabel_max_flow_v4::highest_push_relabel_max_flow_v4(GraphAdjacencyList &G) 
  : Gf(G)
{
  num_nodes = Gf.num_vertices();
  node_excess.resize(num_nodes+1, 0);
  node_height.resize(num_nodes+1, 0);
  bucket.resize(2*(num_nodes+1), std::queue<int>());
}


// Satura todos os arcos que saem da fonte no grafo Residual -> f(s,v) = u(s,v)
void highest_push_relabel_max_flow_v4::initializePreflow()
{
  for (LinkedList::edge *e = Gf.get_edge(src); e != NULL; e = e->next) {
    if (e->capacity > 0) {
      pushFlow(src, e, e->capacity);
    }
  }

  node_height[src] = num_nodes;
}


// Empurra excesso contido no nó u para o nó v
// Note: flow deve ser igual a min(node_excess[u], e->capacity);
void highest_push_relabel_max_flow_v4::pushFlow(int u, LinkedList::edge *e, int flow)
{
  // Atualiza o fluxo e capacidade dos arcos (u,v) e (v,u) no grafo residual
  e->flow += flow;
  Gf.add_flow(e->destNode, u, -flow);

  e->capacity -= flow;
  Gf.add_capacity(e->destNode, u, +flow);

  // Insere o nó que recebeu excesso na fila se ele é diferente de s e t, e não possuia excesso antes da operação
  if (node_excess[e->destNode] == 0 and (e->destNode != src and e->destNode != sink)) {
    bucket[node_height[e->destNode]].push(e->destNode);
  }

  // Atualiza o excesso de u e de v
  node_excess[u] -= flow;
  node_excess[e->destNode] += flow; 

  num_pushs++;
}


// Percorre a lista de todos os arcos que saem de u:
// Dentre os arcos (u,v) com capacidade residual positiva,
// encontra o vértice v com menor rótulo de altura e atualiza
// h(u) para h(v)+1, tornando (u,v) um arco admissível.
void highest_push_relabel_max_flow_v4::relabel(int u)
{
  int h_min = 2 * num_nodes;
  LinkedList::edge *e;

  for (e = Gf.get_edge(u); e != NULL; e = e->next) {
    if (e->capacity > 0) {
      if (node_height[e->destNode] < h_min) {
        h_min = node_height[e->destNode];
      }
    }
  }

  node_height[u] = h_min + 1;

  // Como no algoritmo highest label Push-Relabel pegamos sempre
  // o nó com maior node_height, d* sempre aumenta ao fazer relabel
  if (node_height[u] > d) {
    d = node_height[u];
  }

  num_relabels++;
}


bool highest_push_relabel_max_flow_v4::isAdmissible(int u, LinkedList::edge *e)
{
  return (e->capacity > 0 and node_height[u] == node_height[e->destNode] + 1);
}


// Enquanto um nó u tiver excesso positivo realiza operações de push e de relabel
void highest_push_relabel_max_flow_v4::discharge(int u)
{
  LinkedList::edge *e;

  while (node_excess[u] > 0 and node_height[u] < num_nodes) {
    for (e = Gf.get_edge(u); node_excess[u] > 0 and e != NULL; e = e->next) {
      if (isAdmissible(u, e)) {
        int delta = std::min(node_excess[u], e->capacity);
        pushFlow(u, e, delta);
      }
    }

    if (node_excess[u] > 0) {
      relabel(u);
    }
  }
}


int highest_push_relabel_max_flow_v4::solve(int s, int t)
{
  auto start = std::chrono::steady_clock::now();

  src = s;
  sink = t;
  d = 0;

  initializePreflow();

  while (d >= 0) {
    if (!bucket[d].empty()) {
      int u = bucket[d].front(); bucket[d].pop();
      discharge(u);
    } 
    else {
      d = d - 1;
    }
  }

  //
  max_flow = node_excess[t];

  auto end = std::chrono::steady_clock::now();
  time_execution = (end - start).count();

  return max_flow;
}