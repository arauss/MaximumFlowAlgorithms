#ifndef HEAP_NODE
#define HEAP_NODE

#include <vector>

struct Node {
  int id, priority;
};

class BinaryHeap {
private:
  std::vector<Node> heap;
  std::vector<int> rotulo;
  int size;

public:
  BinaryHeap(int num);
  // ~BinaryHeap(); 

  static int parent(int i) { return (i - 1) / 2; }
  static int leftChild(int i) { return 2 * i + 1; }
  static int rightChild(int i) { return 2 * i + 2; }

  bool empty() { return heap.size() == 0; }

  void insert(Node data);
  void upHeap(int i);
  void downHeap(int i);
  void changePriority(int i, Node data);
  Node extractMax();

  int  getIndexNode(int i);
  Node getNode(int rotuloNode);
  Node getMax();

  void printHeap();
};
  
#endif