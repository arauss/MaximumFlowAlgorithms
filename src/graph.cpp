#include "graph.h"

#include <iostream>
#include <fstream> 
#include <limits.h>
#include <string.h>
#include <vector>
#include <chrono>



// Inicializa um grafo sem arcos
GraphAdjacencyList::GraphAdjacencyList(int numV) {
  n = numV;         
  G = new LinkedList::edge*[n + 1]; 

  for (int u = 0; u <= n; u++) {
    G[u] = NULL;
  }
}

// Constroi um Grafo residual a partir de um grafo original
GraphAdjacencyList::GraphAdjacencyList(GraphAdjacencyList &G_original) {
  this->n = G_original.num_vertices();
  this->G = new LinkedList::edge*[this->n + 1];

  for (int u = 0; u <= this->n; u++) {
    this->G[u] = NULL;
  }

  // Insere os arcos diretos presentes no grafo original
  // e os arcos reversos de capacidade zero caso não exista no grafo original
  for (int u = 0; u <= this->n; u++) {
    LinkedList::edge *e = G_original.get_edge(u);

    while (e != NULL) {
      add_edge(u, e->destNode, e->capacity);    // direct edge

      if (G_original.get_edge(e->destNode, u) == NULL) {
        add_edge(e->destNode, u, 0);            // reverse edge
      }
      
      e = e->next;
    }
  }
}


// Constroi um grafo residual delta a partir de um grafo residual
// utilizando apenas os arcos de capacidade residual maior que delta 
GraphAdjacencyList::GraphAdjacencyList(GraphAdjacencyList &G_residual, int delta) {
  this->n = G_residual.num_vertices();
  this->G = new LinkedList::edge*[this->n + 1];

  // Insere os arcos no grafo residual delta
  for (int u = 0; u <= this->n; u++) {
    this->G[u] = NULL;
    LinkedList::edge *e;

    for (e = G_residual.get_edge(u); e != nullptr; e = e->next) {
      if (e->capacity >= delta) {
        this->add_edge(u, e->destNode, e->capacity); // direct edge

        LinkedList::edge *e_reverse = G_residual.get_edge(e->destNode, u);
        if (e_reverse->capacity < delta) {
          this->add_edge(e->destNode, u, 0);           // reverse edge
        }
      }     
    }
  }
}



// Adiciona um arco com origem u, destino v, e capacidade capacity
void GraphAdjacencyList::add_edge(int u, int v, int capacity) {
  LinkedList::edge *newEdge = new LinkedList::edge;
  newEdge->destNode = v;
  newEdge->capacity = capacity;
  newEdge->next = G[u];

  G[u] = newEdge;
  m++;
}

// Acrescenta capacityEdit unidades na capacidade atual do arco (u,v)
void GraphAdjacencyList::add_capacity(int u, int v, int capacityEdit) {
  LinkedList::edge *e = get_edge(u, v);
  
  if (e != NULL) {
    e->capacity += capacityEdit;
  }
}

// Acrescenta pushFlow unidades no fluxo atual do arco (u,v)
void GraphAdjacencyList::add_flow(int u, int v, int pushFlow) {
  LinkedList::edge *e = get_edge(u, v);

  if (e != NULL) {
    e->flow += pushFlow;
  }
}

// Percorre o grafo definindo todo fluxo como zero
void GraphAdjacencyList::zera_flow() {

  for (int u = 0; u <= n; u++) {
    LinkedList::edge *e = get_edge(u);
    
    while (e != NULL) {
      e->flow = 0;
      e = e->next;
    }
  }
}

// Retorna um apontador para o primeiro arco saindo de u
LinkedList::edge * GraphAdjacencyList::get_edge(int u) { return G[u]; }

// Retorna um apontador para o arco (u,v), caso exista, ou para NULL
LinkedList::edge * GraphAdjacencyList::get_edge(int u, int v) {
  LinkedList::edge *e = G[u];

  while (e != NULL) {
    if(e->destNode == v)
      return e;
    e = e->next;
  }

  return NULL;
}


// Retorna o valor da capacidade do arco (u,v)
int GraphAdjacencyList::get_capacity(int u, int v) {
  LinkedList::edge *e = get_edge(u, v);
  
  if (e != NULL) {
    return (e->capacity);
  }

  return -1;
}

// Retorna o número de vértices contido no grafo
int GraphAdjacencyList::num_vertices() { return this->n; }

// Retorna o número de Arcos contido no grafo
int GraphAdjacencyList::num_edges() { return this->m; }


void GraphAdjacencyList::print_grafo() {
  for (int u = 0; u <= n; u++) {
    LinkedList::edge *e = G[u];

    while (e != NULL) {
      std::cout << "a " << u << " " << e->destNode << " " << e->flow << " " << e->capacity << std::endl;
      e = e->next;
    }
  }
}

GraphAdjacencyList::~GraphAdjacencyList() {
  if (G != NULL) {
    for (int u = 0; u <= n; u++) {
      LinkedList::edge *e_temp = G[u];

      while (e_temp != NULL) {
        LinkedList::edge *prox = e_temp->next;
        delete e_temp;
        e_temp = prox;
      }
    }
    
    delete[] G;
  }
}


























// Inicializa um grafo sem arcos
GraphAdjListWithReverse::GraphAdjListWithReverse(int numV) {
  n = numV;         
  G = new LinkedListWithReverse::edge*[n + 1];

  numEdges = new int[n+1];

  for (int u = 0; u <= n; u++) {
    G[u] = nullptr;
    numEdges[u] = 0;
  }
}

// Constroi um Grafo residual a partir de um grafo original
GraphAdjListWithReverse::GraphAdjListWithReverse(GraphAdjListWithReverse &G_original) {

  this->n = G_original.num_vertices();
  this->G = new LinkedListWithReverse::edge*[this->n + 1];

  this->numEdges = new int[this->n+1];

  for (int u = 0; u <= this->n; u++) {
    this->G[u] = nullptr;
    this->numEdges[u] = 0;
  }
  

  // Paraca arco (i,j) do grafo original verifica se ele já existe no reisidual
  //  - Se existir, atualiza a capacidade desse arco no grafo residual
  //  - Se não, cria os arcos (i,j) e (j,i)-[com cap 0] no grafo residual

  // ver tempo acumulado das buscas
  // verificar se não tem erro
  // multigrafo
  // estrutura auxiliar pra não fazer buscas atoa
  // rodar testes do blist (com todas as melhorias) sem sort e atualizar a tabela
  for (int u = 1; u <= this->n; u++) {
    LinkedListWithReverse::edge *e = G_original.get_edge(u);

    while (e != nullptr) {
      int v = e->destNode;
      LinkedListWithReverse::edge *e_residual;

      if(numEdges[u] <= numEdges[v]) {
        e_residual = this->get_edge(u, v);
      } else {
        e_residual = this->get_edge(v, u);
        if (e_residual != nullptr)
          e_residual = e_residual->reverse;
      }

      if (e_residual != nullptr) {
        e_residual->capacity = e->capacity;
      } else {
        this->add_edge_with_reverse(u, v, e->capacity);
      }

      e = e->next;
    }
  }
}





// Constroi um grafo residual delta Gf(Δ) = (V, AfΔ) a partir de um grafo
// residual Gf = (V, Af), em que AfΔ é o conjunto de todos os arcos (i,j)
// pertencentes a Af cuja capacidade residual seja maior ou igual a um
// parâmetro delta 
GraphAdjListWithReverse::GraphAdjListWithReverse(GraphAdjListWithReverse &G_residual, int delta) {
  this->n = G_residual.num_vertices();
  this->G = new LinkedListWithReverse::edge*[this->n + 1];

  // Insere em GfΔ todos os arcos (i,j) \in Gf tal que uf(i,j) >= delta.
  // Se o arco (i,j) atender à essa propriedade mas seu reverso (j,i)
  // não, então adiciona-se em GfΔ um arco reverso em que uf(j,i) = 0
  for (int u = 0; u < this->n+1; u++) {
    this->G[u] = nullptr;
    
    LinkedListWithReverse::edge *e;
    for (e = G_residual.get_edge(u); e != nullptr; e = e->next) {
      
      if (e->capacity >= delta) {
        this->add_edge(u, e->destNode, e->capacity); // direct edge

        if (e->reverse->capacity < delta) {
          this->add_edge(e->destNode, u, 0);         // reverse edge
        }
      }        
    }
  }
}



// Adiciona um arco com origem u, destino v, e capacidade capacity
void GraphAdjListWithReverse::add_edge(int u, int v, int capacity) {
  LinkedListWithReverse::edge *newEdge = new LinkedListWithReverse::edge;
  newEdge->destNode = v;
  newEdge->capacity = capacity;
  newEdge->next = G[u];

  G[u] = newEdge;
  m++;
}


// Adiciona um arco com origem u, destino v, e capacidade capacity
// e seu reverso como origem v, destino u, e capacidade 0
void GraphAdjListWithReverse::add_edge_with_reverse(int u, int v, int capacity) {
  // alocate and add the (u,v) arc in adjascency list of u
  LinkedListWithReverse::edge *newEdge = new LinkedListWithReverse::edge;
  newEdge->destNode = v;
  newEdge->capacity = capacity;

  newEdge->next = G[u];
  G[u] = newEdge;

  // alocate and add the (v,u) arc in adjascency list of v
  LinkedListWithReverse::edge *reverseEdge = new LinkedListWithReverse::edge;
  reverseEdge->destNode = u;
  reverseEdge->capacity = 0;

  reverseEdge->next = G[v];
  G[v] = reverseEdge;

  // adjust the pointers
  newEdge->reverse = reverseEdge;
  reverseEdge->reverse = newEdge;

  numEdges[u] = numEdges[u] +1;
  numEdges[v] = numEdges[v] +1;
  
  m += 2;
}

// Acrescenta flow unidades no fluxo atual do arco (u,v)
void GraphAdjListWithReverse::add_flow(int u, int v, int flow) {
  LinkedListWithReverse::edge *e = get_edge(u, v);

  if (e != nullptr and e->flow + flow <= e->capacity) {
    e->flow += flow;
  }
}

// Acrescenta capacityEdit unidades na capacidade atual do arco (u,v)
void GraphAdjListWithReverse::add_capacity(int u, int v, int capacityEdit) {
  LinkedListWithReverse::edge *e = get_edge(u, v);
  
  if (e != nullptr) {
    e->capacity += capacityEdit;
  }
}


// Percorre o grafo definindo todo fluxo como zero
void GraphAdjListWithReverse::zera_flow() {

  for (int u = 0; u <= n; u++) {
    LinkedListWithReverse::edge *e = get_edge(u);
    
    while (e != nullptr) {
      e->flow = 0;
      e = e->next;
    }
  }
}

// Retorna um apontador para o primeiro arco saindo de u
LinkedListWithReverse::edge * GraphAdjListWithReverse::get_edge(int u) { return G[u]; }

// Retorna um apontador para o arco (u,v), caso exista, ou para nullptr
LinkedListWithReverse::edge * GraphAdjListWithReverse::get_edge(int u, int v) {
  LinkedListWithReverse::edge *e = G[u];

  while (e != nullptr) {
    if(e->destNode == v)
      return e;
    e = e->next;
  }

  return nullptr;
}


// Retorna o valor da capacidade do arco (u,v)
int GraphAdjListWithReverse::get_capacity(int u, int v) {
  LinkedListWithReverse::edge *e = get_edge(u, v);
  
  if (e != nullptr) {
    return (e->capacity);
  }

  return -1;
}

// Retorna o número de vértices contido no grafo
int GraphAdjListWithReverse::num_vertices() { return this->n; }

// Retorna o número de Arcos contido no grafo
int GraphAdjListWithReverse::num_edges() { return this->m; }


void GraphAdjListWithReverse::print_grafo() {
  for (int u = 0; u <= n; u++) {
    LinkedListWithReverse::edge *e = G[u];

    while (e != nullptr) {
      std::cout << "a " << u << " " << e->destNode << " " << e->flow << " " << e->capacity << "\n";
      e = e->next;
    }
  }
}

GraphAdjListWithReverse::~GraphAdjListWithReverse() {
  if (G != nullptr) {
    for (int u = 0; u <= n; u++) {
      LinkedListWithReverse::edge *e_temp = G[u];

      while (e_temp != NULL) {
        LinkedListWithReverse::edge *prox = e_temp->next;
        delete e_temp;
        e_temp = prox;
      }
    }
    
    delete[] G;
  }
}





















GraphAdjacencyVector::GraphAdjacencyVector(int numV) {
  n = numV;
  G = std::vector<std::vector<vectorList::edge>> (n + 1);
}


// Constroi um Grafo residual a partir de um grafo original
GraphAdjacencyVector::GraphAdjacencyVector(GraphAdjacencyVector &originalGraph) {
  this->n = originalGraph.num_vertices();
  this->G = std::vector<std::vector<vectorList::edge>> (n+1);

  // Insere os arcos diretos presentes no grafo original
  // e os arcos reversos de capacidade zero caso não exista no grafo original
  for (int u = 0; u < this->n + 1; u++) {
    std::vector<vectorList::edge> &v = originalGraph.get_edge_list(u);

    for (vectorList::edge e : v) {
      add_edge(u, e.destNode, e.flow, e.capacity);   // direct edge

      if (originalGraph.get_edge_pointer(e.destNode, u) == NULL) {
        add_edge(e.destNode, u, 0, 0);               // reverse edge
      }
    }
  }
}

// Adiciona um arco (u,v) na lista de arcos que saem de u
void GraphAdjacencyVector::add_edge(int u, int v, int flow, int capacity) {
  G[u].push_back( vectorList::edge{v, flow, capacity} );
  m++;
}

// Acrescenta capacityEdit unidades na capacidade atual do arco (u,v)
void GraphAdjacencyVector::add_capacity(int u, int v, int capacityEdit) {
  vectorList::edge *e = get_edge_pointer(u, v);
  
  if (e != NULL) {
    e->capacity += capacityEdit;
  }
}

// Acrescenta pushFlow unidades no fluxo atual do arco (u,v)
void GraphAdjacencyVector::add_flow(int u, int v, int pushFlow) {
  vectorList::edge *e = get_edge_pointer(u, v);

  if (e != NULL and e->flow + pushFlow <= e->capacity) {
    e->flow += pushFlow;
  }
}

// Acrescenta pushFlow unidades no fluxo atual do arco (u,v)
void GraphAdjacencyVector::update_flow_capacity(int u, int v, int delta) {
  vectorList::edge *e = get_edge_pointer(u, v);

  if (e != NULL) {
    e->flow += delta;
    e->capacity -= delta;
  }
}

// Retorna um apontador para o primeiro arco saindo de u
std::vector<vectorList::edge> & GraphAdjacencyVector::get_edge_list(int u) {
  return G[u]; 
}

// Retorna um apontador para o arco (u,v), caso exista, ou para NULL, caso contrário
vectorList::edge * GraphAdjacencyVector::get_edge_pointer(int u, int v) {
  for (int i = 0; i < G[u].size(); i++) {
    if(G[u][i].destNode == v)
      return &(G[u][i]);
  }
  
  return nullptr;
}

// Retorna o número de vértices contido no grafo
int GraphAdjacencyVector::num_vertices() { return this->n; }

// Retorna o número de Arcos contido no grafo
int GraphAdjacencyVector::num_edges() { return this->m; }


// percorre o grafo e imprime no formato: identificador origem destino capacidade
void GraphAdjacencyVector::print_grafo() {
  for (int u = 1; u < n + 1; u++) {
    for (vectorList::edge e: G[u]) {
      std::cout << "a " << u << " " << e.destNode << " " << e.flow << " " << e.capacity << "\n";
    }
  }
}

GraphAdjacencyVector::~GraphAdjacencyVector() {
  // não é necessário desalocar memória
}