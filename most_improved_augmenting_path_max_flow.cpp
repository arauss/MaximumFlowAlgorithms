#include "graph.cpp"
#include "./dependencies/heapNode.cpp"

#include <algorithm>
#include <iostream>

using namespace std;

/* Algoritmo de Edmonds-Karp: Most Improved Augmenting Path */

int widest_capacity_path(ListGraph &Gf, int s, int t, int parent[]) {

  Node u, v, no;
  int delta;
  int num_vertices = Gf.num_vertices();

  int capacity[num_vertices + 1] = {};
  int visited[num_vertices + 1] = {};
  BinaryHeap pq(num_vertices + 1);

  for (int i = 0; i <= num_vertices; i++) {
    parent[i] = -1;
  }

  // Inicializa a capacidade de todos os (s,u)-caminhos como -inf
  // Exceto para o caso (s,s)-caminho que possui capacidade ilimitada
  for (int u = 1; u <= num_vertices; u++) {
    capacity[u] = INT_MIN;
  }
  capacity[s] = INT_MAX;

  // Insere s na fila de nós encontrados
  no.id = s;
  no.capacityPath = capacity[s];
  pq.insert(no);

  parent[s] = -1;

  
  while (!pq.empty()) {

    // remove o nó u que possui a maior capacidade de (s,u)-caminho e marca como visitado
    u = pq.extractMax(); 
    visited[u.id] = 1;

    // Se u for igual a t, então t foi visitado e atribuída sua maior capacidade de (s-t)-caminho
    // Armazena na posição 0 do vetor parent o valor da capacidade caminho até t
    if (u.id == t) {
      parent[0] = u.capacityPath;
      break;
    }

    // Se não:
    LinkedListImplementation::edge *aux = Gf.get_edge(u.id);

    /* para todos os nos v alcançaveis a partir de u
    verifica a capacidade do caminho de s até u e o arco (u,v).
    Se a capacidade desse caminho até v utilizando o arco (u,v)
    for melhor que a capacidade do caminho atual até v, 
    então v recebe o novo parent e nova capacidade              */
    while (aux != NULL) {

      if (aux->capacity > 0) {
        delta = min(u.capacityPath, aux->capacity);

        if (delta > capacity[aux->destNode] and visited[aux->destNode] != 1) {
          parent[aux->destNode] = u.id;
          capacity[aux->destNode] = delta;

          v.id = aux->destNode;
          v.capacityPath = delta;

          pq.insert(v);
        }
      }

      aux = aux->next;
    }
  }

  //pq.~BinaryHeap();

  // retorna uma confirmação de que t foi alcançado
  return (parent[t] != -1);
}

int most_improved_augmenting_path_max_flow(ListGraph &G, int s, int t, int *numPaths) {
  
  int u, v, capacity_path = 0, flow = 0;
  int parent[G.num_vertices() + 1];
  ListGraph Gf(G);

  // Empurra fluxo enquanto existir um (s,t)-caminho
  while (widest_capacity_path(Gf, s, t, parent)) {
    
    // capacity_path = INT_MAX;
    // Encontra a capacidade residual mínima do caminho encontrado
    // for (v = t; v != s; v = parent[v]) {
    //   u = parent[v];
    //   capacity_path = min(capacity_path, Gf.getCapacityEdge(u, v));
    // }

    // Define a capacidade residual mínima do caminho encontrado
    capacity_path = parent[0];

    for (v = t; v != s; v = parent[v]) {
      u = parent[v];
      Gf.add_capacity(u, v, -capacity_path);
      Gf.add_capacity(v, u, +capacity_path);
    }

    flow += capacity_path;
    *numPaths += 1;
  }

  return flow;
}



// // Algorithm Widest Capacity Path
// int WCP(Graph &Gf, int s, int t, int parent[]) {
//   Node u, v, no;
//   int delta;
//   int capacity[Gf.getNumVertices()] = {};
//   int visited[Gf.getNumVertices()] = {};
//   BinaryHeap pq(Gf.getNumVertices());

//   for (int i = 0; i < Gf.getNumVertices(); i++) {
//     capacity[i] = INT_MIN;
//   }
//   capacity[s] = INT_MAX;

//   no.id = s;
//   no.capacityPath = capacity[s];
//   pq.insert(no);

//   parent[s] = -1;

//   for (int i = 1; i < Gf.getNumVertices(); i++) {
//     no.id = i;
//     no.capacityPath = capacity[i];
//     pq.insert(no);
//   }
  
//   /* Se u for igual a t, então t foi visitado e possui sua capacidade max de (s-t)-caminho
//   Se a capacidade de u == -inf, todos os nós restantes na fila não são alcançáveis a partir de s  */
//   while (!pq.empty() and u.id != t and u.capacityPath != INT_MIN) {

//     u = pq.extractMax(); // remove o nó u com maior capacidade de (s,u)-caminho
//     visited[u.id] = 1;

//     if (u.capacityPath != INT_MIN) {
      
//       Edge *aux = Gf.getEdge(u.id); // pega o primeiro arco saindo de u

//       /* para todos os nos v alcançaveis a partir de u
//       verifica a capacidade do caminho de s até u e o arco (u,v).
//       Se a capacidade desse caminho até v utilizando o arco (u,v)
//       for melhor que a capacidade do caminho atual até v, 
//       então v recebe o novo parent e nova capacidade              */
//       while (aux != NULL) {

//         if (aux->capacity > 0) {
//           delta = min(u.capacityPath, aux->capacity);

//           if (delta > capacity[aux->destNode] and visited[aux->destNode] != 1) {
//             parent[aux->destNode] = u.id;
//             capacity[aux->destNode] = delta;

//             v.id = aux->destNode;
//             v.capacityPath = delta;

//             pq.insert(v);
//           }
//         }

//         aux = aux->next;
//       }
//       fflush(stdin);
//     }
//   }
//   //pq.~BinaryHeap();

//   // retorna uma confirmação de que t foi alcançado
//   return (parent[t] != 0);
// }

// int maxFlowBAP(Graph &G, int s, int t, int *numPaths) {
//   int u, v, capacity_path = 0, flow = 0;
//   int parent[G.getNumVertices()] = {};
//   Graph Gf(G);

//   // Empurra fluxo enquanto existir um (s,t)-caminho
//   while (WCP(Gf, s, t, parent)) {

//     capacity_path = INT_MAX;

//     // Encontra a capacidade residual mínima do caminho encontrado
//     for (v = t; v != s; v = parent[v]) {
//       u = parent[v];
//       capacity_path = min(capacity_path, Gf.getCapacityEdge(u, v));
//     }

//     for (v = t; v != s; v = parent[v]) {
//       u = parent[v];
//       Gf.addCapacityEdge(u, v, -capacity_path);
//       Gf.addCapacityEdge(v, u, +capacity_path);
//     }

//     // cout << flow << endl;
//     flow += capacity_path;
//     memset(parent, 0, sizeof(parent));
//     fflush(stdin);

//     *numPaths += 1;
//   }

//   return flow;
// }

