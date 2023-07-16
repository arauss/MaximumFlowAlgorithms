#ifndef LIST
#define LIST

#include <queue>
#include <iostream>


typedef struct height_queue HQ;
struct height_queue {
  int heightLabel;
  std::queue<int> queueNodeLabel;
  HQ *next;
};


class ListHeight {
  HQ *header = nullptr;
  HQ *alocateNode(int altura, int u);

public:
  void pushElement(int altura, int u);
  int popFrontElement();
  bool empty();
  void printList();
};

#endif