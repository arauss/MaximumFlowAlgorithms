#include "graph.cpp"
#include "./dependencies/heapNode.cpp"

#include <queue>
#include <algorithm>
#include <chrono>

/* Algoritmo de Goldberg e Tarjan: Push Relabel */

void preflowInitialize(ListGraph &G, ListGraph &Gf, int altura[], int excesso[], BinaryHeap &pq, int s, int t) {
  
  // satura todos os arcos que saem da fonte no grafo original -> f(s,v) = u(s,v)
  for (LinkedListImplementation::edge *aux = Gf.get_edge(s); aux != NULL; aux = aux->next) {

    // Atualiza o excesso do nó que recebeu fluxo e o insere na fila de prioridade de altura
    if (aux->destNode != t and aux->capacity > 0) {
      excesso[aux->destNode] = aux->capacity;
      pq.insert( Node {aux->destNode, altura[aux->destNode]} );
    }

    // Atualiza as capacidades residuais dos arcos diretos e reversos
    Gf.add_capacity(aux->destNode, s, +aux->capacity);
    aux->capacity -= aux->capacity;
  }

  altura[s] = Gf.num_vertices();

  return;
}

void PushFlow(ListGraph &G, ListGraph &Gf, int u, LinkedListImplementation::edge *aux, \
int altura[], int excesso[], BinaryHeap &pq, int s, int t) {
  
  // Calcula a quantidade de excesso que é possível enviar
  int delta = min(excesso[u], aux->capacity);
  
  // G.addFlowEdge(u, aux->destNode, +delta);
  // G.addFlowEdge(aux->destNode, u, -delta);
  
  // Atualiza a capacidade dos arcos (u,v) e (v,u) no grafo residual
  aux->capacity -= delta;
  Gf.add_capacity(aux->destNode, u, delta);

  // Insere o nó que recebeu excesso na fila se ele é diferente de s e t, e não possuia excesso antes da operação
  if (excesso[aux->destNode] == 0 and (aux->destNode != s and aux->destNode != t)) {
    pq.insert( Node {aux->destNode, altura[aux->destNode]} );
  }

  // Atualiza o excesso de u
  excesso[u] -= delta;

  // Se u perde todo seu excesso ele é removido da lista
  if (excesso[u] == 0) {
    pq.extractMax();
  }
  
  // Atualiza o excesso v somente se v != s,t
  if (aux->destNode != s and aux->destNode != t) {
    excesso[aux->destNode] += delta;
  }

  return;
}

void Relabel(ListGraph &Gf, int u, int altura[]) {
  // Para todos os arcos (u,v) com capacidade residual positiva,
  // encontra o vértice v com menor rótulo de altura e atualiza
  // a altura de u para (u,v) se torne um arco admissível.
  
  int h_min = INT_MAX;
  for (LinkedListImplementation::edge *aux = Gf.get_edge(u); aux != NULL; aux = aux->next) {
    if (aux->capacity > 0) {
      h_min = min(h_min, altura[aux->destNode] + 1);
    }
  }

  altura[u] = h_min;

  return;
}

LinkedListImplementation::edge * existAdmissibleEdge(ListGraph &Gf, int u, int altura[]) {

  // busca um arco admissível saindo de u
  for (LinkedListImplementation::edge *aux = Gf.get_edge(u); aux != NULL; aux = aux->next) {
    if (altura[u] == altura[aux->destNode] + 1 and aux->capacity > 0) {
      return aux; // retorna o destino do arco admissível
    }
  }

  return NULL; // retorna indicador de que não existe arco admssível com origem em u
}


int push_relabel_max_flow(ListGraph &G, int s, int t) {
  int flow = 0;
  int len = G.num_vertices() + 1;
  int excesso[len] = {}; // armazena o excesso presente em cada nó
  int altura[len] = {};  // armazena o rótulo de altura de cada nó

  ListGraph Gf(G);           // Cria um grafo residual a partir de G
  BinaryHeap pq(len);    // fila de priorides que armazena altura de nós com excesso positivo

  preflowInitialize(G, Gf, altura, excesso, pq, s, t);

  // Empura fluxo nos arcos admissíveis enquanto existir um vértice com execesso positivo
  // Vértice com execesso positivo == ter um vértice na fila de prioridades
  while (!pq.empty()) {
    
    Node u = pq.getMax();
    LinkedListImplementation::edge *aux = existAdmissibleEdge(Gf, u.id, altura);
   
    if (aux != NULL) {
      PushFlow(G, Gf, u.id, aux, altura, excesso, pq, s, t);
    } 
    else {
      Relabel(Gf, u.id, altura);

      u.capacityPath = altura[u.id];
      pq.changePriority(pq.getIndexNode(u.id), u);
    }
  }

  // Calcula o fluxo que sai fonte
  for (LinkedListImplementation::edge *aux = G.get_edge(s); aux != NULL; aux = aux->next) {
    LinkedListImplementation::edge *auxResidual = Gf.get_edge(s, aux->destNode);
    flow += aux->capacity - auxResidual->capacity;
  }
  
  return flow;
}