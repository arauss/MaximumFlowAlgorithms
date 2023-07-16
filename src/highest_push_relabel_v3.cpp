#include "graph.h"
#include "highest_push_relabel_v3.h"
#include "insertionSort.h"

#include <vector>
#include <algorithm>



highest_push_relabel_max_flow_v3::highest_push_relabel_max_flow_v3(GraphAdjacencyVector &G)
: Gf(G)
{
  num_nodes = Gf.num_vertices();
  node_excess.resize(num_nodes+1, 0);
  node_height.resize(2*(num_nodes+1), 0);
}


void highest_push_relabel_max_flow_v3::initializePreflow() {
  std::vector<vectorList::edge> &edgeList = Gf.get_edge_list(src);
  int sizeList = edgeList.size();
    
  // satura todos os arcos que saem da fonte no grafo Original -> f(s,v) = u(s,v)
  for (int i = 0; i < sizeList; i++) {
    if (edgeList[i].capacity > 0){
      pushFlow(src, &edgeList[i], edgeList[i].capacity);
    }
  }

  node_height[src] = num_nodes;
}


void highest_push_relabel_max_flow_v3::pushFlow(int u, vectorList::edge *e, int flow) {
  // Atualiza o fluxo e capacidade dos arcos (u,v) e (v,u) no grafo residual
  e->flow += flow;
  e->capacity -= flow;

  Gf.update_flow_capacity(e->destNode, u, -flow);


  // Insere o nó que recebeu excesso na fila se ele é diferente de s e t, e não possuia excesso antes da operação
  if (node_excess[e->destNode] == 0 and (e->destNode != src and e->destNode != sink)) {
    bucket.pushElement(node_height[e->destNode], e->destNode);
  }

  // Atualiza o excesso de u e de v
  node_excess[u] -= flow;
  node_excess[e->destNode] += flow; 

  num_pushs++;
}


void highest_push_relabel_max_flow_v3::relabel(int u, vectorList::edge e) {
  node_height[u] = node_height[e.destNode] + 1;
  num_relabels++;
}


void highest_push_relabel_max_flow_v3::discharge(int u) {

  // Obtém a lista de arcos que saem de u
  std::vector<vectorList::edge> &edgeList = Gf.get_edge_list(u);

  // Ordena a lista de arcos que saem de u pela altura de cada destNode
  insertionSort(edgeList, node_height);

  // Percorre a lista de arcos que saem de u
  for (int i = 0; node_excess[u] > 0 and i < edgeList.size(); i++) {
    vectorList::edge e = edgeList[i];

    if (e.capacity > 0) {
      // Relabel: ao encontrar um nó com node_height igual ou maior que u, atualiza a node_height de u
      // e faz com que a próxima iteração inicie nesse arco novamente
      if (node_height[e.destNode] >= node_height[u]) {
        relabel(u, e);
      }
      
      // Push: ao encontrar um arco (u,v) em que v está um nível abaixo de u
      if (node_height[u] == node_height[e.destNode] + 1) {
        int delta = std::min(node_excess[u], e.capacity);
        pushFlow(u, &edgeList[i], delta);
      }
    }
  }
}


int highest_push_relabel_max_flow_v3::solve(int s, int t) {
  auto start = std::chrono::steady_clock::now();

  src = s;
  sink = t;

  initializePreflow();

   // Descarrega o excesso enquanto houver nós ativos
  while (!bucket.empty()) {
    int u = bucket.popFrontElement();
    discharge(u);
  }


  // Calcula o fluxo que sai fonte
  std::vector<vectorList::edge> &edgeList = Gf.get_edge_list(src);
  
  for (int i = 0; i < edgeList.size(); i++) {
    max_flow += edgeList[i].flow;
  }

  auto end = std::chrono::steady_clock::now();
  time_execution = (end - start).count();

  return max_flow;
}




// int highest_label_push_relabel_max_flow_v3(VectorGraph &G, int s, int t) {
//   auto start = std::chrono::steady_clock::now();

//   VectorGraph Gf(G);

//   int flow = 0;
//   int u = 0;
//   int n = G.num_vertices();

//   vector<int> excesso(n + 1);  // armazena a quantidade de excesso presente em cada nó
//   vector<int>  altura(n + 1);  // armazena o valor do rótulo de altura de cada nó
//   ListHeight b;                // armazena na fila de rótulo k o conjunto de nós ativos com altura d(k)

//   operations_highest_label_push_relabel_max_flow_v3::preflowInitialize(Gf, excesso, altura, b, s, t);
  
//   // Descarrega o excesso enquanto houver nós ativos
//   while (!b.empty()) {
//     u = b.popFrontElement();
//     operations_highest_label_push_relabel_max_flow_v3::discharge(u, Gf, excesso, altura, b, s, t);
//   }

//   // Calcula o fluxo que sai fonte
//   vector<vectorList::edge> &edgeList = G.get_edge_list(s);

//   for (int i = 0; i < edgeList.size(); i++) {
//     vectorImplementation::edge *e_original = &edgeList[i];
//     vectorImplementation::edge *e_residual = Gf.get_edge_pointer(s, e_original->destNode);

//     flow += e_original->capacity - e_residual->capacity;
//   }

//   auto end = std::chrono::steady_clock::now();
//   long int time_ = (end - start).count();
//   std::cout << "tempo de execucao: " << time_ << std::endl;
//   return flow;
// }




// namespace operations_highest_label_push_relabel_max_flow_v3 {

//   void preflowInitialize(VectorGraph &Gf, std::vector<int> &excesso, vector<int> &altura, ListHeight &b, int s, int t) {
//     // satura todos os arcos que saem da fonte no grafo Original -> f(s,v) = u(s,v)
//     vector<vectorImplementation::edge> &edgeList = Gf.get_edge_list(s);
//     int sizeList = edgeList.size();
    
//     for (int i = 0; i < sizeList; i++) {
//       vectorImplementation::edge *e = &edgeList[i];

//       // Atualiza o excesso do nó que recebeu fluxo e o insere no bucket de nós que possuem altura 0
//       if (e->capacity > 0 and e->destNode != t) {
//         excesso[e->destNode] = e->capacity;
//         b.pushElement(0, e->destNode);
//       }

//       // Atualiza as capacidades residuais dos arcos diretos e reversos
//       Gf.add_capacity(e->destNode, s, +e->capacity);
//       e->capacity -= e->capacity;
//     } 

//     altura[s] = Gf.num_vertices();
    
//     return;
//   }


//   void pushFlow(VectorGraph &Gf, int u, vectorImplementation::edge *e, vector<int> &excesso, vector<int> &altura, ListHeight &b, int s, int t) {
    
//     // Calcula a quantidade de excesso que é possível enviar
//     int delta = min(excesso[u], e->capacity);
    
//     // Atualiza a capacidade dos arcos (u,v) e (v,u) no grafo residual
//     e->capacity -= delta;
//     Gf.add_capacity(e->destNode, u, delta);

//     // Insere o nó v que recebeu excesso no bucket de índice d(v)
//     // se ele não possuia excesso antes da operação e é diferente de s e t
//     if (excesso[e->destNode] == 0 and (e->destNode != s and e->destNode != t)) {
//       b.pushElement(altura[e->destNode], e->destNode);
//     }

//     // Atualiza o excesso de u
//     excesso[u] -= delta;

//     // Atualiza o excesso recebido por v
//     if (e->destNode != s and e->destNode != t) {
//       excesso[e->destNode] += delta; 
//     }

//     return;
//   }

//   void relabel(int u, vectorImplementation::edge e, vector<int> &altura) {
//     altura[u] = altura[e.destNode] + 1;  
//     return;
//   }


//   void discharge(int u, VectorGraph &Gf, vector<int> &excesso, vector<int> &altura, ListHeight &b, int s, int t) {

//     vector<vectorImplementation::edge> &edgeList = Gf.get_edge_list(u);

//     // Ordena a lista de arcos que saem de u com base na altura atual de cada nó destino desses arcos
//     // quickSort(edgeList, 0, sizeList - 1, altura);
//     insertionSort(edgeList, altura);
    
//     // Percorre a lista de arcos que saem de u
//     for (int i = 0; excesso[u] > 0 and i < edgeList.size(); i++) {
//       vectorImplementation::edge e = edgeList[i];

//       if (e.capacity > 0) {
//         // Push: ao encontrar um arco (u,v) em que v está um nível abaixo de u, transfere excesso de u para v
//         if (altura[u] == altura[e.destNode] + 1) {
//           pushFlow(Gf, u, &edgeList[i], excesso, altura, b, s, t);
//         }

//         // Relabel: ao encontrar um arco (u,v) em que v possui altura igual ou maior que a altura de u,
//         // atualiza a altura de u e faz com que a próxima iteração inicie nesse arco novamente
//         else if (altura[e.destNode] >= altura[u]) {
//           relabel(u, e, altura);
//           i--;
//         }
//       }
//     }

//     return;
//   }

// }


// int highest_label_push_relabel_max_flow_v3(VectorGraph &G, int s, int t) {
//   auto start = std::chrono::steady_clock::now();

//   VectorGraph Gf(G);

//   int flow = 0;
//   int u = 0;
//   int n = G.num_vertices();

//   vector<int> excesso(n + 1);  // armazena a quantidade de excesso presente em cada nó
//   vector<int>  altura(n + 1);  // armazena o valor do rótulo de altura de cada nó
//   ListHeight b;                // armazena na fila de rótulo k o conjunto de nós ativos com altura d(k)

//   operations_highest_label_push_relabel_max_flow_v3::preflowInitialize(Gf, excesso, altura, b, s, t);
  
//   // Descarrega o excesso enquanto houver nós ativos
//   while (!b.empty()) {
//     u = b.popFrontElement();
//     operations_highest_label_push_relabel_max_flow_v3::discharge(u, Gf, excesso, altura, b, s, t);
//   }

//   // Calcula o fluxo que sai fonte
//   vector<vectorImplementation::edge> &edgeList = G.get_edge_list(s);

//   for (int i = 0; i < edgeList.size(); i++) {
//     vectorImplementation::edge *e_original = &edgeList[i];
//     vectorImplementation::edge *e_residual = Gf.get_edge_pointer(s, e_original->destNode);

//     flow += e_original->capacity - e_residual->capacity;
//   }

//   auto end = std::chrono::steady_clock::now();
//   long int time_ = (end - start).count();
//   std::cout << "tempo de execucao: " << time_ << std::endl;
//   return flow;
// }