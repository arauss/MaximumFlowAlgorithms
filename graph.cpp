#pragma once

#include <iostream>
#include <fstream> 
#include <limits.h>
#include <string.h>
#include <queue>

using namespace std;

typedef struct edge Edge; 
struct edge {
  int destNode, flow = 0, capacity;
  Edge *next;
};

class Graph {
  int m = 0, n = 0; // nro de arcos e vértices do grafo
  Edge **G;         // vetor de apontadores (Vertices do grafo) 
  /* G aponta para um vértice que apontam para uma lista de vértices (Arcos) */

public:
  Graph(int numVertices);
  Graph(string archiveName);
  Graph(Graph &G_original);
  Graph(Graph &G_original, int delta);
  ~Graph();

  void addEdge(int u, int v, int capacity);
  void addFlowEdge(int u, int v, int flow);
  void addCapacityEdge(int u, int v, int capacityEdit);
  void zeraFlow();

  Edge *getEdge(int u);
  Edge *getEdge(int u, int v);
  int getCapacityEdge(int u, int v);
  int getNumVertices();
  int getNumEdges();

  void printGrafo();
  void printGrafoArchive();
};

// Inicializa um grafo sem arcos
Graph::Graph(int numVertices) {
  n = numVertices;  // seta o nro de vértices do grafo
  G = new Edge*[n+1]; // aloca um n-vetor de apontadores para o tipo node

  for (int u = 0; u <= n; u++)
    G[u] = NULL;
}

// retorna uma palavra (substring) que vai de startIndex até o primeiro separator encontrado
int getWord(string str, int startIndex, char separator, string *word) { 
  int i = startIndex, endIndex;

  while (i <= str.size()) { 
    if (str[i] == separator || i == str.size())  {  
      endIndex = i;  
      *word = "";
      word->append(str, startIndex, endIndex - startIndex);
      return endIndex + 1;
    } 
    i++;  
  }
  return 0;  
}

// Constroi um grafo a partir de um arquivo de texto no formato
// marcador vetice_origem vertice_destino capacidade \n
Graph::Graph(string archiveName) {
  string word, line;
  char separator = ' ';
  int startIndex = 0;
  int u, v, capacity;

  // abertura do arquivo
  ifstream file;
  file.open(archiveName, ios::in);

  if(!file) {
    cout << "Erro na abertura do arquivo.\n";
    return;
  }

  // Lê o nro de nós do grafo
  getline(file, line);
  startIndex = getWord(line, startIndex, separator, &word); // problem
  startIndex = getWord(line, startIndex, separator, &word); // type
  startIndex = getWord(line, startIndex, separator, &word); // nro nodes
  
  n = stoi(word);
  G = new Edge*[n + 1]; // aloca um n-vetor de apontadores para o tipo node
  for (int u = 0; u <= n; u++)  G[u] = NULL;

  // Percorre o arquivo e inserindo cada arco no grafo
  while (!file.eof()) { 
    startIndex = 0;
    getline(file, line);

    if (line[0] == 'a') {
      startIndex = 2;
      while (startIndex < line.length()) {
        startIndex = getWord(line, startIndex, separator, &word);
        u = stoi(word);

        startIndex = getWord(line, startIndex, separator, &word);
        v = stoi(word);

        startIndex = getWord(line, startIndex, separator, &word);
        capacity = stoi(word);

        addEdge(u, v, capacity);
      }
    }
  }

  file.close();
}


// Constroi um Grafo residual a partir de um grafo original
Graph::Graph(Graph &G_original) {
  this->n = G_original.getNumVertices();
  this->G = new Edge*[this->n + 1];

  for (int u = 0; u <= this->n; u++) {
    this->G[u] = NULL;
  }

  // Insere os arcos diretos presentes no grafo original
  // e os arcos reversos de capacidade zero caso não exista no grafo original
  for (int u = 0; u <= this->n; u++) {
    Edge *aux = G_original.getEdge(u);

    while (aux != NULL) {
      addEdge(u, aux->destNode, aux->capacity);   // direct edge

      if (G_original.getEdge(aux->destNode, u) == NULL) {
        addEdge(aux->destNode, u, 0);             // reverse edge
      }
      
      aux = aux->next;
    }
  }
}

// Constroi um grafo residual delta a partir de um grafo residual
// utilizando apenas os arcos de capacidade residual maior que delta 
Graph::Graph(Graph &G_residual, int delta) {
  this->n = G_residual.getNumVertices();
  this->G = new Edge*[this->n + 1];

  if (delta > 1) {
    // Insere os arcos no grafo residual delta
    for (int u = 0; u <= this->n; u++) {
      this->G[u] = NULL;
      Edge *aux = G_residual.getEdge(u);

      while (aux != NULL) {
        if (aux->capacity >= delta) {
          addEdge(u, aux->destNode, aux->capacity); // direct edge

          Edge *bwd = G_residual.getEdge(aux->destNode, u);
          if (bwd->capacity < delta) {
            addEdge(aux->destNode, u, 0);           // reverse edge
          }
        }
                    
        aux = aux->next;
      }
    }

  } else { // Gf(delta) = Gf

    for (int u = 0; u <= this->n; u++) {
      this->G[u] = NULL;
      Edge *aux = G_residual.getEdge(u);

      while (aux != NULL) {
        addEdge(u, aux->destNode, aux->capacity); // direct edge         
        aux = aux->next;
      }
    }
  }
}


// Adiciona um arco com origem u, destino v, e capacidade capacity
void Graph::addEdge(int u, int v, int capacity) {
  Edge *newEdge = new Edge;
  newEdge->destNode = v;
  newEdge->capacity = capacity;
  newEdge->next = G[u];

  G[u] = newEdge;
  m++;
}

// Acrescenta capacityEdit unidades na capacidade atual do arco (u,v)
void Graph::addCapacityEdge(int u, int v, int capacityEdit) {
  Edge * aux = getEdge(u, v);
  
  if (aux != NULL) {
    aux->capacity += capacityEdit;
  }
}

// Acrescenta pushFlow unidades no fluxo atual do arco (u,v)
void Graph::addFlowEdge(int u, int v, int pushFlow) {
  Edge * aux = getEdge(u, v);

  if (aux != NULL and aux->flow + pushFlow <= aux->capacity) {
    aux->flow += pushFlow;
  }
}

// Percorre o grafo definindo todo fluxo como zero
void Graph::zeraFlow() {

  for (int u = 0; u <= n; u++) {
    Edge *aux = getEdge(u);
    
    while (aux != NULL) {
      aux->flow = 0;
      aux = aux->next;
    }
  }
}

// Retorna um apontador para o primeiro arco saindo de u
Edge * Graph::getEdge(int u) { return G[u]; }

// Retorna um apontador para o arco (u,v), caso exista, ou para NULL
Edge * Graph::getEdge(int u, int v) {
  Edge * aux = G[u];

  while (aux != NULL) {
    if(aux->destNode == v)
      return aux;
    aux = aux->next;
  }

  return NULL;
}

// Retorna o número de vértices contido no grafo
int Graph::getNumVertices() { return this->n; }

// Retorna o número de Arcos contido no grafo
int Graph::getNumEdges() { return this->m; }

// Retorna o valor da capacidade do arco (u,v)
int Graph::getCapacityEdge(int u, int v) {
  Edge * aux = getEdge(u, v);
  
  if (aux != NULL) {
    return (aux->capacity);
  }

  return -1;
}


// percorre o grafo e imprime no formato (origem, destino, capacidade)
void Graph::printGrafo() {
  for (int u = 0; u <= n; u++) {
    Edge *aux = G[u];

    while (aux != NULL) {
      cout << "(" << u << ", " << aux->destNode << ", " << aux->flow << ", " << aux->capacity << ")\n";
      aux = aux->next;
    }
  }
  cout << "\n";
}

void Graph::printGrafoArchive() {
  for (int u = 0; u <= n; u++) {
    Edge *aux = G[u];

    while (aux != NULL) {
      cout << "a " << u << " " << aux->destNode << " " << aux->flow << " " << aux->capacity << endl;
      aux = aux->next;
    }
  }
}


Graph::~Graph() {
  Edge *aux; 

  for (int u = 0; u <= n; u++) { 
    aux = G[u];

    while (aux != NULL) { 
      G[u] = G[u]->next;
      delete aux;
      aux = G[u];
    }

    //G[u] = NULL; 
  }

  delete [] G; 
  G = NULL; 
}


/**** Métodos Desativados ****/

// Constro um Grafo usando um arquivo no formato
// vetice_origem vertice_destino1 capacidade1 vertice_destino2 capacidade2, ...
//
// Graph::Graph(string archiveName) {
//   string word, line;
//   char separator = ' ';
//   int startIndex = 0;
//   int u, v, capacity;

//   // abertura do arquivo
//   ifstream file;
//   file.open(archiveName, ios::in);

//   if(!file) {
//     cout << "Erro na abertura do arquivo.\n";
//     return;
//   }

//   // Lê o nro de nós do grafo
//   getline(file, line);
//   n = stoi(line);
//   G = new Edge*[n]; // aloca um n-vetor de apontadores para o tipo node

//   // Percorre o arquivo e inserindo cada arco no grafo
//   for (u = 0; u < n; u++) { 
//     G[u] = NULL;
//     startIndex = 0;
//     getline(file, line);

//     while (startIndex < line.length()) {
//       startIndex = getWord(line, startIndex, separator, &word);
//       v = stoi(word);
//       startIndex = getWord(line, startIndex, separator, &word);
//       capacity = stoi(word);

//       addEdge(u, v, capacity);
//     }
//   }
  
//   file.close();
// }

// Constroi um grafo residual com os arcos diretos e reversos a partir de um grafo original
// Graph::Graph(Graph &G_original) {
//   this->n = G_original.getNumVertices();
//   this->G = new Edge*[this->n];

//   for (int u = 0; u < this->n; u++) {
//     this->G[u] = NULL;
//   }

//   // Insere os arcos diretos e reversos no grafo residual
//   for (int u = 0; u < this->n; u++) {
//     Edge *aux = G_original.getEdge(u);

//     while (aux != NULL) {
//       addEdge(u, aux->destNode, aux->capacity); // direct edge
//       addEdge(aux->destNode, u, 0);             // reverse edge
//       aux = aux->next;
//     }
//   }
// }