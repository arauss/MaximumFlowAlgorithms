#ifndef HIGHEST_PUSH_RELABEL_V3
#define HIGHEST_PUSH_RELABEL_V3

#include "graph.h"
#include "quickSort.h"
#include "insertionSort.h"
#include "list.h"

#include <vector>

class highest_push_relabel_max_flow_v3 {
  GraphAdjacencyVector Gf;
  int src, sink;
  int num_nodes;

  int max_flow = 0;
  long num_pushs = 0;
  long num_relabels = 0;
  long time_execution = 0;
  
  std::vector<int> node_excess;
  std::vector<int> node_height;
  ListHeight bucket;

  void initializePreflow();
  void pushFlow(int u, vectorList::edge *e, int flow);
  void relabel(int u, vectorList::edge e);
  void discharge(int u);

public:
  // ctor
  highest_push_relabel_max_flow_v3(GraphAdjacencyVector &G);

  int solve(int s, int t);

  int get_max_flow() { return max_flow; };
  long get_num_pushs() { return num_pushs; };
  long get_num_relabels() { return num_relabels; };
  long get_time_execution() { return time_execution; };
};


#endif