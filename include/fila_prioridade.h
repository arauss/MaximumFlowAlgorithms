#ifndef HIGHEST_PUSH_RELABEL_V3
#define HIGHEST_PUSH_RELABEL_V3


typedef struct No2 {
  int prioridade, rotulo;
  struct No2 *fwd, *bwd; // abreviation forward and backward
} no;

class Fila {
  int n; 
  No2 *q;

public:
  Fila();

  void insertElement(int rotulo, int prioridade);
  void changePriority(int rot, int prio);
  No2 removeElement();
  No2 getElement();
  int getTam() {return n;};

  bool empty();
  void printFila();
};

#endif