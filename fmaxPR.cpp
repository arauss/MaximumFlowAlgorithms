#include "graph.cpp"
#include "heapNode.cpp"

#include <queue>
#include <algorithm>
#include <chrono>

void preflowInitialize(Graph &G, Graph &Gf, int altura[], int excesso[], BinaryHeap &pq, int s, int t) {
  
  Edge *aux = G.getEdge(s);

  // satura todos os arcos que saem da fonte (f(s,v) = u(s,v)) no grafo Original
  while (aux != NULL) {
    // aux->flow = aux->capacity;

    // Atualiza o excesso do nó que recebeu fluxo e o insere na fila de prioridade de altura
    if (aux->destNode != t and aux->capacity > 0) {

      excesso[aux->destNode] = aux->capacity;
      Node novo;
      novo.id = aux->destNode;
      novo.capacityPath = altura[aux->destNode];
      pq.insert(novo);
    }

    // Atualiza as capacidades residuais dos arcos diretos e reversos
    Gf.addCapacityEdge(s, aux->destNode, -aux->capacity);
    Gf.addCapacityEdge(aux->destNode, s, +aux->capacity);

    aux = aux->next;
  }

  altura[s] = Gf.getNumVertices();

  return;
}

void PushFlow(Graph &G, Graph &Gf, int u, Edge *aux, int altura[], int excesso[], \
BinaryHeap &pq, int s, int t) {
  
  // Empurra fluxo no arco (u,v)
  int delta = min(excesso[u], aux->capacity);
  
  // G.addFlowEdge(u, aux->destNode, +delta);
  // G.addFlowEdge(aux->destNode, u, -delta);
  
  // Atualiza a capacidade dos arcos (u,v) e (v,u) no grafo residual
  aux->capacity -= delta;
  Gf.addCapacityEdge(aux->destNode, u, delta);

  // Insere o nó que recebeu excesso na fila se ele já não possuia excesso antes
  // E também se o nó que recebeu o excesso é diferente de s e t
  if (excesso[aux->destNode] == 0 and (aux->destNode != s and aux->destNode != t)) {
    Node novo;
    novo.id = aux->destNode;
    novo.capacityPath = altura[aux->destNode];
    pq.insert(novo);
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

void Relabel(Graph &Gf, int u, int altura[]) {
  Edge *aux = Gf.getEdge(u);
  int h_min = INT_MAX;

  // Para todos os arcos (u,v) com capacidade residual positiva,
  // encontra o vértice v com menor rótulo de altura e atualiza
  // a altura de u para (u,v) se torne um arco admissível.
  while (aux != NULL) {
    if (aux->capacity > 0) {
      h_min = min(h_min, altura[aux->destNode]);
    }
    aux = aux->next;
  }

  altura[u] = h_min + 1;
  return;
}

Edge * existAdmissibleEdge(Graph &Gf, int u, int altura[]) {
  Edge *aux = Gf.getEdge(u);

  // busca um arco admissível saindo de u
  while (aux != NULL) {
    if (altura[u] == altura[aux->destNode] + 1 and aux->capacity > 0)
      return aux; // retorna o destino do arco admissível
    aux = aux->next;
  }

  return NULL; // retorna indicador de que não existe arco admssível com origem em u
}


int maxFlowPR(Graph &G, int s, int t) {
  int flow = 0;
  int len = G.getNumVertices();
  int excesso[len] = {}; // armazena o excesso presente em cada nó
  int altura[len] = {};  // armazena o rótulo de altura de cada nó

  Graph Gf(G);          // Cria um grafo residual a partir de G
  BinaryHeap pq(len);   // fila de priorides que armazena altura de nós com excesso positivo

  preflowInitialize(G, Gf, altura, excesso, pq, s, t);

  // Empura fluxo nos arcos admissíveis enquanto existir um vértice com execesso positivo
  // vértice com execesso positivo == ter um vértice na fila de prioridades
  while (!pq.empty()) {
    
    Node u = pq.getMax();

    Edge *aux = existAdmissibleEdge(Gf, u.id, altura);
   
    if (aux != NULL) {

      PushFlow(G, Gf, u.id, aux, altura, excesso, pq, s, t);
      
    } else {

      Relabel(Gf, u.id, altura);
      
      u.capacityPath = altura[u.id];
      pq.changePriority(pq.getIndexNode(u.id), u);
    
    }
  }

  Edge *aux = G.getEdge(s);
  Edge *auxResidual;

  while (aux != NULL) {
    auxResidual = Gf.getEdge(s, aux->destNode);

    flow += aux->capacity - auxResidual->capacity;
    aux = aux->next;
  }
  
  return flow;
}