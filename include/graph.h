#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <limits.h>
#include <vector>
#include <chrono>


namespace LinkedList {
  struct edge {
    int destNode, flow = 0, capacity;
    edge *next;
  };
}

class GraphAdjacencyList {
  int m = 0, n = 0;
  LinkedList::edge **G; 

public:
  GraphAdjacencyList(int numV);
  GraphAdjacencyList(GraphAdjacencyList &G_original);
  GraphAdjacencyList(GraphAdjacencyList &G_original, int delta);
  ~GraphAdjacencyList();

  void add_edge(int u, int v, int capacity);
  void add_flow(int u, int v, int flow);
  void add_capacity(int u, int v, int capacityEdit);
  void zera_flow();

  LinkedList::edge *get_edge(int u);
  LinkedList::edge *get_edge(int u, int v);

  int num_vertices();
  int num_edges();
  int get_capacity(int u, int v);
  
  void print_grafo();
};





// Grafo implementado no formato semi-lista de adjascência:
// - Um vector que representa o conjuto de nós do grafo
// - Cada nó (posição do vector) armazena um vector contendo os out edges referente a ele
namespace vectorList {
  struct edge {
    int destNode = -1;
    int flow = 0;
    int capacity = 0;

    // Construtor
    edge(int dest, int f, int cap) : destNode(dest), flow(f), capacity(cap) {}
  };
}

class GraphAdjacencyVector {
  int m = 0, n = 0;
  std::vector<std::vector<vectorList::edge>> G;

public:
  GraphAdjacencyVector(int numV);
  GraphAdjacencyVector(GraphAdjacencyVector &originalGraph);
  ~GraphAdjacencyVector();

  void add_edge(int u, int v, int flow, int capacity);
  void add_flow(int u, int v, int pushFlow);
  void add_capacity(int u, int v, int capacityEdit);
  void update_flow_capacity(int u, int v, int delta);

  vectorList::edge *get_edge_pointer(int u, int v);
  std::vector<vectorList::edge> & get_edge_list(int u);

  int num_vertices();
  int num_edges();
  
  void print_statistics();
  void print_grafo();
};





// Grafo implementado no formato lista de adjascência:
// - Um vector que representa o conjuto de nós do grafo
// - Cada nó (posição do vector) armazena um ponteiro para o primeiro edge
//      da lista encadeada de out edges referente a ele
namespace LinkedListWithReverse {
  struct edge {
    int destNode, flow = 0, capacity;
    edge *reverse = nullptr;
    edge *next;
  };
}

class GraphAdjListWitchReverse {
  int m = 0, n = 0;
  LinkedListWithReverse::edge **G; 

public:
  GraphAdjListWitchReverse(int numV);
  GraphAdjListWitchReverse(GraphAdjListWitchReverse &G_original);
  GraphAdjListWitchReverse(GraphAdjListWitchReverse &G_original, int delta);
  ~GraphAdjListWitchReverse();

  void add_edge(int u, int v, int capacity);
  void add_edge_with_reverse(int u, int v, int capacity);
  void add_flow(int u, int v, int flow);
  void add_capacity(int u, int v, int capacityEdit);
  void zera_flow();

  LinkedListWithReverse::edge *get_edge(int u);
  LinkedListWithReverse::edge *get_edge(int u, int v);

  int num_vertices();
  int num_edges();
  int get_capacity(int u, int v);
  
  void print_grafo();
};

#endif // GRAPH_H