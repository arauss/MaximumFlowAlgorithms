#include "graph.cpp"

#include "./dependencies/getWord.cpp"
#include "most_improved_augmenting_path_max_flow.cpp"
#include "shortest_augmenting_path_max_flow.cpp"
#include "capacity_scaling_max_flow.cpp"
#include "push_relabel_max_flow.cpp"
#include "highest_label_push_relabel_max_flow_v1.cpp"
#include "highest_label_push_relabel_max_flow_v2.cpp"
#include "highest_label_push_relabel_max_flow_v3.cpp"

#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <math.h>

using namespace std;

// ./instancias/BVZ-tsukuba0.max

int main(int argc, char **argv) {

  if(argc < 2) {
    cout << "Numero incorreto de parâmetros. \n";
    cout << "Exemplo: " << argv[0] << " inst01.in \n";
    return 0;
  }

  // abertura do arquivo
  ifstream file;
  file.open(argv[1], ios::in);

  if(!file) {
    cout << "Erro na abertura do arquivo.\n";
    return 0;
  }

  //

  string word, label, type, line;
  char separator = ' ';
  int startIndex = 0;
  int n = 0, s = 0, t = 0;
  int u, v, capacity;
  int numPaths = 0;

  // Lê o nro de vértices do grafo e o rótulo dos vétices fonte e terminal
  while (n == 0 or s == 0 or t == 0) {
    getline(file, line);
    startIndex = 2;

    if (line[0] == 'p') {
      startIndex = getWord(line, startIndex, separator, &word); // type problem
      startIndex = getWord(line, startIndex, separator, &word); // nro nodes
      n = stoi(word);
    }

    if (line[0] == 'n') {
      startIndex = getWord(line, startIndex, separator, &label);
      startIndex = getWord(line, startIndex, separator, &type);

      if (type == "s")
        s = stoi(label);
      else if (type == "t")
        t = stoi(label);
    }
  }

  ListGraph G_list(n);
  VectorGraph G_vector(n);

  // Percorre o arquivo e insere cada arco no grafo
  while (!file.eof()) { 
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

        G_list.add_edge(u, v, capacity);
        G_vector.add_edge(u, v, 0, capacity);
      }
    }
  }

  //

  int fmax = 0;
  auto start = chrono::steady_clock::now();

  if (strcmp(argv[2], "BAP") == 0) {
    fmax = most_improved_augmenting_path_max_flow(G_list, s, t, &numPaths);
  }
  else if (strcmp(argv[2], "CS") == 0) {
    fmax = capacity_scaling_max_flow(G_list, s, t, &numPaths);
  }
  else if (strcmp(argv[2], "SAP") == 0) {
    fmax = shortest_augmenting_path_max_flow(G_list, s, t, &numPaths);
  }
  else if (strcmp(argv[2], "PR") == 0) {
    fmax = push_relabel_max_flow(G_list, s, t);
  }
  else if (strcmp(argv[2], "HLPRv1") == 0) {
    fmax = highest_label_push_relabel_max_flow_v1(G_list, s, t);
  }
  else if (strcmp(argv[2], "HLPRv2") == 0) {
    fmax = highest_label_push_relabel_max_flow_v2(G_vector, s, t);
  }
  else if (strcmp(argv[2], "HLPRv3") == 0) {
    fmax = highest_label_push_relabel_max_flow_v3(G_vector, s, t);
  }

  auto end = chrono::steady_clock::now();
  auto duration = (end - start).count();

  //

  int numV = 0, numE = 0;

  if (strcmp(argv[2], "BAP") == 0 or strcmp(argv[2], "CS") == 0 or strcmp(argv[2], "SAP") == 0 or \
  strcmp(argv[2], "PR") == 0 or strcmp(argv[2], "HLPRv1") == 0) {
    numV =  G_list.num_vertices();
    numE = G_list.num_edges();
  }

  else if (strcmp(argv[2], "HLPRv2") == 0 or strcmp(argv[2], "HLPRv3") == 0) {
    numV =  G_vector.num_vertices();
    numE = G_vector.num_edges();
  }

  cout << "Vertex: " << numV << endl;
  cout << "Arcs: " << numE << endl;
  cout << "Flow: " << fmax << endl; 
  cout << "Time: " << duration << endl;

  if (strcmp(argv[2], "BAP") == 0 or strcmp(argv[2], "CS") == 0 or strcmp(argv[2], "SAP") == 0) {
    cout << "Paths: " << numPaths << endl;
  }
  
  return 0;
}