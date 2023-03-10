#include "graph.cpp"
#include "fmaxBAP.cpp"
#include "fmaxSAP.cpp"
#include "fmaxCS.cpp"
#include "fmaxPR.cpp"

#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <math.h>

using namespace std;

// ./instancias/BVZ-tsukuba0.max

int main(int argc, char **argv) {
  string label, type, line;
  char separator = ' ';
  int startIndex = 0;
  int s = 0, t = 0, numPaths = 0;


  if(argc < 2) {
    cout << "Numero incorreto de parâmetros. \n";
    cout << "Exemplo: " << argv[0] << " inst01.in \n";
    return 0;
  }

  auto initGrafo = chrono::steady_clock::now();
  Graph G(argv[1]);
  auto endGrafo = chrono::steady_clock::now();

  auto x = endGrafo - initGrafo;
  // cout << "tempo leitura grafo: " << x.count() << endl;

  

  // abertura do arquivo
  ifstream file;
  file.open(argv[1], ios::in);

  if(!file) {
    cout << "Erro na abertura do arquivo.\n";
    return 0;
  }

  while (s == 0 or t == 0) {
    getline(file, line);

    if (line[0] == 'n') {
      startIndex = 2;

      startIndex = getWord(line, startIndex, separator, &label);
      startIndex = getWord(line, startIndex, separator, &type);

      if (type == "s")
        s = stoi(label);
      else if (type == "t")
        t = stoi(label);
    }
  }

  // cout << s << "," << t << endl;

  int fmax = 0;
  auto start = chrono::steady_clock::now();

  if (strcmp(argv[2], "BAP") == 0) {
    fmax = maxFlowBAP(G, s, t, &numPaths);
  }
  else if (strcmp(argv[2], "CS") == 0) {
    fmax = maxFlowCS(G, s, t, &numPaths);
  }
  else if (strcmp(argv[2], "SAP") == 0) {
    fmax = maxFlowSAP(G, s, t, &numPaths);
  }
  else if (strcmp(argv[2], "PR") == 0) {
    fmax = maxFlowPR(G, s, t);
  }

  auto end = chrono::steady_clock::now();
  auto elapsed = end - start;

  cout << "fluxo: " << fmax << endl; 
  cout << "tempo: " << elapsed.count() << endl;
  if (strcmp(argv[2], "BAP") == 0 or strcmp(argv[2], "CS") == 0 or strcmp(argv[2], "SAP") == 0) {
    cout << "Caminhos: " << numPaths << endl;
  }
  cout << "Vertices: " << G.getNumVertices() << endl;
  cout << "Arcos: " << G.getNumEdges() << endl;
  cout << "\n";
  //G.printGrafoArchive();

  // G.~Graph();
  return 0;
}