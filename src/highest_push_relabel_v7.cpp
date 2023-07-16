#include <vector>
#include <chrono>

#include "graph.h"
#include "highest_push_relabel_v7.h"
#include "priority_queue.h"
#include "list_node_height.h"

highest_push_relabel_max_flow_v7::highest_push_relabel_max_flow_v7(GraphAdjListWitchReverse &G) 
  : Gf(G), list_height(Gf.num_vertices())
{
  num_nodes = Gf.num_vertices();
  node_excess.resize(num_nodes+1, 0);
  node_height.resize(num_nodes+1, 0);
}


// Satura todos os arcos que saem da fonte no grafo Residual -> f(s,v) = u(s,v)
void highest_push_relabel_max_flow_v7::initializePreflow()
{
  for (LinkedListWithReverse::edge *e = Gf.get_edge(src); e != NULL; e = e->next) {
    if (e->capacity > 0) {
      pushFlow(src, e, e->capacity);
    }
  }

  node_height[src] = num_nodes;
  list_height.update_h(src, num_nodes);
}



// Empurra excesso contido no nó u para o nó v
// Note: flow deve ser igual a min(node_excess[u], e->capacity);
void highest_push_relabel_max_flow_v7::pushFlow(int u, LinkedListWithReverse::edge *e, int flow)
{
  // Atualiza o fluxo e capacidade dos arcos (u,v) e (v,u) no grafo residual
  e->flow += flow;
  e->reverse->flow -= flow;

  e->capacity -= flow;
  e->reverse->capacity += flow;

  // Insere o nó que recebeu excesso na fila se ele é diferente de s e t, e não possuia excesso antes da operação
  if (node_excess[e->destNode] == 0 and (e->destNode != src and e->destNode != sink)) {
    bucket.push(e->destNode, node_height[e->destNode]);
  }

  // Atualiza o excesso de u e de v
  node_excess[u] -= flow;
  node_excess[e->destNode] += flow; 

  num_pushs++;
}


// Se existir algum valor k < n tal que existam nós ativos i com rótulo de distância h(i) > k,
// mas nenhum nó j com rótulo de distância h(j) = k (includo os não ativos),
// então definimos h(i) = n para cada nó i com k < h(i) < n.
void highest_push_relabel_max_flow_v7::gap(int k)
{
  if (k >= num_nodes) return;

  while (list_height.empty_h(k) == false)
  {
    int u = list_height.front_h(k);
    list_height.update_h(u, num_nodes);
    node_height[u] = num_nodes;
  }

  if (list_height.empty_h(k+1) == false)
    gap(k+1);
}


void highest_push_relabel_max_flow_v7::relabel(int u)
{
  // backup da altura atual de u
  int h_u = node_height[u];

  int h_min = 2 * num_nodes;
  LinkedListWithReverse::edge *e;

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

  // Verifica se existe um Gap nessa altura
  if (list_height.empty_h(h_u) == true) {
    gap(h_u+1);
    node_height[u] = num_nodes;
    num_gaps++;
  }

  num_relabels++;
}


bool highest_push_relabel_max_flow_v7::isAdmissible(int u, LinkedListWithReverse::edge *e)
{
  return (e->capacity > 0 and node_height[u] == node_height[e->destNode] + 1);
}


// Enquanto um nó u tiver excesso positivo realiza operações de push e de relabel
void highest_push_relabel_max_flow_v7::discharge(int u)
{
  LinkedListWithReverse::edge *e;
  list_height.remove_h(u);

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

  list_height.update_h(u, node_height[u]);
}


int highest_push_relabel_max_flow_v7::solve(int s, int t)
{
  auto start = std::chrono::steady_clock::now();


  src = s;
  sink = t;
  d = 0;

  list_height.initialize_all_height(0);
  bucket.clear();

  initializePreflow();
      
  while (!bucket.is_empty()) {
    int u = bucket.pop();
    discharge(u);
  }

  max_flow = node_excess[t];

  auto end = std::chrono::steady_clock::now();
  time_execution = (end - start).count();

  return max_flow;
}