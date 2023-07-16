#include "graph.h"
#include "getWord.h"

#include "most_improved_augmenting_path.h"
#include "capacity_scaling.h"
#include "shortest_augmenting_path.h"
#include "push_relabel.h"
#include "highest_push_relabel_v1.h"
#include "highest_push_relabel_v2.h"
#include "highest_push_relabel_v3.h"
#include "highest_push_relabel_v4.h"
#include "highest_push_relabel_v5.h"
#include "highest_push_relabel_v6.h"
#include "highest_push_relabel_v7.h"

#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <math.h>

using namespace std;


int main(int argc, char **argv)
{
  if(argc < 2) {
    cout << "Numero incorreto de parâmetros. \n";
    cout << "Exemplo: " << argv[0] << " inst01.in \n";
    return 0;
  }

  // Open to file instance
  ifstream file(argv[1]);

  if (!file.is_open()) {
    cout << "erro na abertura do arquivo.\n\n";
    return EXIT_FAILURE;
  }


  //
  string algorithm = argv[2];
  string word, label, type, line;
  char separator = ' ';
  unsigned startIndex = 0;
  int n = 0, s = 0, t = 0;
  int u, v, capacity;

  // Lê o nro de vértices do grafo e o rótulo dos vétices fonte e terminal
  getline(file, line);
  startIndex = 6;
  startIndex = getWord(line, startIndex, separator, &word); // nro nodes
  n = stoi(word);

  getline(file, line);
  startIndex = 2;
  startIndex = getWord(line, startIndex, separator, &label); // s
  s = stoi(label);

  getline(file, line);
  startIndex = 2;
  startIndex = getWord(line, startIndex, separator, &label); // t
  t = stoi(label);

  GraphAdjacencyList G_simple(n);
  ListGraph G_list(n);
  VectorGraph G_vector(n);

  // Percorre o arquivo e insere cada arco no grafo
  while (getline(file, line)) { 

    if (line[0] == 'a') {
      startIndex = 2;
      while (startIndex < line.length()) {
        startIndex = getWord(line, startIndex, separator, &word);
        u = stoi(word);

        startIndex = getWord(line, startIndex, separator, &word);
        v = stoi(word);

        startIndex = getWord(line, startIndex, separator, &word);
        capacity = stoi(word);

        G_simple.add_edge(u, v, capacity);
        G_list.add_edge(u, v, capacity);
        G_vector.add_edge(u, v, 0, capacity);
      }
    }
  }


  int fmax = 0;
  int num_paths = 0;
  long time_execution;

  GraphAdjacencyList Gf_simple(G_simple);

  if (algorithm == "BAP") {
    auto start = std::chrono::steady_clock::now();
      fmax = most_improved_augmenting_path_max_flow(Gf_simple, s, t, &num_paths);
    auto end = std::chrono::steady_clock::now();
    time_execution = (end - start).count();

    cout << "Arcos: " << G_list.num_edges() << endl;
    cout << "Vertices: " << G_list.num_vertices() << endl; 
    cout << "Fluxo: " << fmax << endl;
    cout << "Tempo(ns): " << time_execution << endl;
    cout << "Caminhos: " << num_paths << endl;
  }

  else if (algorithm == "CS") {
    auto start = std::chrono::steady_clock::now();
      fmax = capacity_scaling_max_flow(Gf_simple, s, t, &num_paths);
    auto end = std::chrono::steady_clock::now();
    time_execution = (end - start).count();

    cout << "Arcos: " << G_list.num_edges() << endl;
    cout << "Vertices: " << G_list.num_vertices() << endl; 
    cout << "Fluxo: " << fmax << endl;
    cout << "Tempo(ns): " << time_execution << endl;
    cout << "Caminhos: " << num_paths << endl;
  }

  else if (algorithm == "SAP") {
    auto start = std::chrono::steady_clock::now();
      fmax = shortest_augmenting_path_max_flow(Gf_simple, s, t, &num_paths);
    auto end = std::chrono::steady_clock::now();
    time_execution = (end - start).count();

    cout << "Arcos: " << G_list.num_edges() << endl;
    cout << "Vertices: " << G_list.num_vertices() << endl; 
    cout << "Fluxo: " << fmax << endl;
    cout << "Tempo(ns): " << time_execution << endl;
    cout << "Caminhos: " << num_paths << endl;
  }

  else if (algorithm == "PR") {
    push_relabel_max_flow max_flow(G_simple);
    fmax = max_flow.solve(s, t);

    cout << "Arcos: " << G_simple.num_edges() << endl;
    cout << "Vertices: " << G_simple.num_vertices() << endl; 
    cout << "Fluxo: " << max_flow.get_max_flow() << endl;
    cout << "Tempo(ns): " << max_flow.get_time_execution() << endl;
    cout << "Pushs: " << max_flow.get_num_pushs() << endl;
    cout << "Relabels: " << max_flow.get_num_relabels() << endl;
  }

  else if (algorithm == "HPRv1") {
    highest_push_relabel_max_flow_v1 max_flow(G_simple);
    fmax = max_flow.solve(s, t);

    cout << "Arcos: " << G_simple.num_edges() << endl;
    cout << "Vertices: " << G_simple.num_vertices() << endl; 
    cout << "Fluxo: " << max_flow.get_max_flow() << endl;
    cout << "Tempo(ns): " << max_flow.get_time_execution() << endl;
    cout << "Pushs: " << max_flow.get_num_pushs() << endl;
    cout << "Relabels: " << max_flow.get_num_relabels() << endl;
  }

  else if (algorithm == "HPRv2") {
    highest_push_relabel_max_flow_v2 max_flow(G_vector);
    fmax = max_flow.solve(s, t);

    cout << "Arcos: " << G_vector.num_edges() << endl;
    cout << "Vertices: " << G_vector.num_vertices() << endl; 
    cout << "Fluxo: " << max_flow.get_max_flow() << endl;
    cout << "Tempo(ns): " << max_flow.get_time_execution() << endl;
    cout << "Pushs: " << max_flow.get_num_pushs() << endl;
    cout << "Relabels: " << max_flow.get_num_relabels() << endl;
  }

  else if (algorithm == "HPRv3") {
    highest_push_relabel_max_flow_v3 max_flow(G_vector);
    fmax = max_flow.solve(s, t);

    cout << "Arcos: " << G_vector.num_edges() << endl;
    cout << "Vertices: " << G_vector.num_vertices() << endl; 
    cout << "Fluxo: " << max_flow.get_max_flow() << endl;
    cout << "Tempo(ns): " << max_flow.get_time_execution() << endl;
    cout << "Pushs: " << max_flow.get_num_pushs() << endl;
    cout << "Relabels: " << max_flow.get_num_relabels() << endl;
  }

  else if (algorithm == "HPRv4") {
    highest_push_relabel_max_flow_v4 max_flow(G_simple);
    fmax = max_flow.solve(s, t);

    cout << "Arcos: " << G_simple.num_edges() << endl;
    cout << "Vertices: " << G_simple.num_vertices() << endl; 
    cout << "Fluxo: " << max_flow.get_max_flow() << endl;
    cout << "Tempo(ns): " << max_flow.get_time_execution() << endl;
    cout << "Pushs: " << max_flow.get_num_pushs() << endl;
    cout << "Relabels: " << max_flow.get_num_relabels() << endl;
  }

  else if (algorithm == "HPRv5") {
    highest_push_relabel_max_flow_v5 max_flow(G_list);
    fmax = max_flow.solve(s, t);

    cout << "Arcos: " << G_list.num_edges() << endl;
    cout << "Vertices: " << G_list.num_vertices() << endl; 
    cout << "Fluxo: " << max_flow.get_max_flow() << endl;
    cout << "Tempo(ns): " << max_flow.get_time_execution() << endl;
    cout << "Pushs: " << max_flow.get_num_pushs() << endl;
    cout << "Relabels: " << max_flow.get_num_relabels() << endl;
    cout << "Gaps: " << max_flow.get_num_gaps() << endl;
  }

  else if (algorithm == "HPRv6") {
    highest_push_relabel_max_flow_v6 max_flow(G_list);
    fmax = max_flow.solve(s, t);

    cout << "Arcos: " << G_list.num_edges() << endl;
    cout << "Vertices: " << G_list.num_vertices() << endl; 
    cout << "Fluxo: " << max_flow.get_max_flow() << endl;
    cout << "Tempo(ns): " << max_flow.get_time_execution() << endl;
    cout << "Pushs: " << max_flow.get_num_pushs() << endl;
    cout << "Relabels: " << max_flow.get_num_relabels() << endl;
    cout << "Gaps: " << max_flow.get_num_gaps() << endl;
  }

  else if (algorithm == "HPRv7") {
    highest_push_relabel_max_flow_v7 max_flow(G_list);
    fmax = max_flow.solve(s, t);

    cout << "Arcos: " << G_list.num_edges() << endl;
    cout << "Vertices: " << G_list.num_vertices() << endl; 
    cout << "Fluxo: " << max_flow.get_max_flow() << endl;
    cout << "Tempo(ns): " << max_flow.get_time_execution() << endl;
    cout << "Pushs: " << max_flow.get_num_pushs() << endl;
    cout << "Relabels: " << max_flow.get_num_relabels() << endl;
    cout << "Gaps: " << max_flow.get_num_gaps() << endl;
  }
  
  return 0;
}