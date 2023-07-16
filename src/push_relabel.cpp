// Uma implementação do algoritmo Highest Push-Relabel para o problema de fluxo máximo.
// Proposto por Goldberg e Tarjan
// Tempo de pior caso: O(n^2 * sqrt(m))

// Sobre a versão:
// Incluímos a operação de discharge que sempre realiza pushs e relabels sobre
// o nó ativo com maior rótulo de altura até que seu excesso seja zerado.

#include "graph.h"
#include "push_relabel.h"
#include "heapNode.h"

#include <vector>
#include <algorithm>
#include <chrono>


push_relabel_max_flow::push_relabel_max_flow(GraphAdjacencyList &G)
  : Gf(G), pq(G.num_vertices()+1) 
{
  num_nodes = Gf.num_vertices();
  node_excess.resize(num_nodes+1, 0);
  node_height.resize(num_nodes+1, 0);
}


// satura todos os arcos que saem da fonte no grafo Original -> f(s,v) = u(s,v)
void push_relabel_max_flow::initializePreflow()
{
  for (LinkedList::edge *e = Gf.get_edge(src); e != NULL; e = e->next) {
    if (e->capacity > 0) {
      pushFlow(src, e, e->capacity);
    }
  }
  
  node_height[src] = num_nodes;
}


void push_relabel_max_flow::pushFlow(int u, LinkedList::edge *e, int flow)
{
  // Atualiza o fluxo e capacidade dos arcos (u,v) e (v,u) no grafo residual
  e->flow += flow;
  Gf.add_flow(e->destNode, u, -flow);
  //e->reverse->flow -= flow;

  e->capacity -= flow;
  Gf.add_capacity(e->destNode, u, +flow);
  //e->reverse->capacity += flow;

  // Insere o nó que recebeu excesso na fila se ele é diferente de s e t, e não possuia excesso antes da operação
  if (node_excess[e->destNode] == 0 and (e->destNode != src and e->destNode != sink)) {
    pq.insert( Node {e->destNode, node_height[e->destNode]} );
  }

  // Atualiza o excesso de u e de v
  node_excess[u] -= flow;
  node_excess[e->destNode] += flow;

  // Se u perde todo seu excesso ele é removido da lista
  if (node_excess[u] == 0) {
    pq.extractMax();
  }

  num_pushs++;
}


void push_relabel_max_flow::relabel(int u) {
  // Para todos os arcos (u,v) com capacidade residual positiva,
  // encontra o vértice v com menor rótulo de altura e atualiza
  // a altura de u para que (u,v) se torne um arco admissível.

  int h_min = 2 * num_nodes+1;

  LinkedList::edge *e;
  for (e = Gf.get_edge(u); e != NULL; e = e->next) {
    if (e->capacity > 0) {
      if (node_height[e->destNode] < h_min) {
        h_min = node_height[e->destNode];
      }
    }
  }

  node_height[u] = h_min + 1;
  num_relabels++;
}


LinkedList::edge * push_relabel_max_flow::existAdmissibleEdge(int u) {

  // busca um arco admissível saindo de u
  LinkedList::edge *e;
  for (e = Gf.get_edge(u); e != NULL; e = e->next) {
    if (node_height[u] == node_height[e->destNode] + 1 and e->capacity > 0) {
      return e; // retorna o destino do arco admissível
    }
  }

  return NULL; // retorna indicador de que não existe arco admssível com origem em u
}



int push_relabel_max_flow::solve(int s, int t) {
  auto start = std::chrono::steady_clock::now();

  src = s;
  sink = t;

  initializePreflow();

  // Empura fluxo nos arcos admissíveis enquanto existir um vértice com execesso positivo
  // Vértice com execesso positivo == ter um vértice na fila de prioridades
  while (!pq.empty()) {
    
    Node u = pq.getMax();
    LinkedList::edge *e = existAdmissibleEdge(u.id);
   
    if (e != NULL) {
      int delta = std::min(node_excess[u.id], e->capacity);
      pushFlow(u.id, e, delta);
    } 
    else {
      relabel(u.id);

      u.priority = node_height[u.id];

      pq.changePriority(pq.getIndexNode(u.id), u);
    }
  }

  // Calcula o fluxo que sai fonte
  LinkedList::edge *e;
  for (e = Gf.get_edge(src); e != NULL; e = e->next) {
    max_flow += e->flow;
  }

  auto end = std::chrono::steady_clock::now();
  time_execution = (end - start).count();

  return max_flow;
}