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

#include "graph.h"
#include "highest_push_relabel_v6.h"
#include "list_node_height.h"

#include <vector>
#include <queue>
#include <algorithm>
#include <chrono>


highest_push_relabel_max_flow_v6::highest_push_relabel_max_flow_v6(GraphAdjListWitchReverse &G) 
  : Gf(G), list_height(Gf.num_vertices())
{
  num_nodes = Gf.num_vertices();
  node_excess.resize(num_nodes+1, 0);
  node_height.resize(num_nodes+1, 0);
  bucket.resize(2 * (num_nodes+1), std::queue<int>());
}


// Satura todos os arcos que saem da fonte no grafo Residual -> f(s,v) = u(s,v)
void highest_push_relabel_max_flow_v6::initializePreflow()
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
void highest_push_relabel_max_flow_v6::pushFlow(int u, LinkedListWithReverse::edge *e, int flow)
{
  // Atualiza o fluxo e capacidade dos arcos (u,v) e (v,u) no grafo residual
  e->flow += flow;
  e->reverse->flow -= flow;

  e->capacity -= flow;
  e->reverse->capacity += flow;

  // Insere o nó que recebeu excesso na fila se ele é diferente de s e t, e não possuia excesso antes da operação
  if (node_excess[e->destNode] == 0 and (e->destNode != src and e->destNode != sink)) {
    bucket[node_height[e->destNode]].push(e->destNode);
  }

  // Atualiza o excesso de u e de v
  node_excess[u] -= flow;
  node_excess[e->destNode] += flow; 

  num_pushs++;
}


// Se existir algum valor k < n tal que existam nós ativos i com rótulo de distância h(i) > k,
// mas nenhum nó j com rótulo de distância h(j) = k (includo os não ativos),
// então definimos h(i) = n para cada nó i com k < h(i) < n.
// Para essa rotina, assume-se que foi verificado que a altura k possui um gap
// então perocorre-se as listas de nós acima da altura k atualizando
// sua altura para n até que um gap na altura l > k seja encontrado
// neste caso, epla construção do algoritmo, este gap já foi tratado
// anteriormente.
void highest_push_relabel_max_flow_v6::gap(int k)
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


void highest_push_relabel_max_flow_v6::relabel(int u)
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


bool highest_push_relabel_max_flow_v6::isAdmissible(int u, LinkedListWithReverse::edge *e)
{
  return (e->capacity > 0 and node_height[u] == node_height[e->destNode] + 1);
}


// Enquanto um nó u tiver excesso positivo realiza operações de push e de relabel
void highest_push_relabel_max_flow_v6::discharge(int u)
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


int highest_push_relabel_max_flow_v6::solve(int s, int t)
{
  auto start = std::chrono::steady_clock::now();

  src = s;
  sink = t;
  d = 0;

  list_height.initialize_all_height(0);
  //list_height.print();
  
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