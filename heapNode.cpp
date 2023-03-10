#pragma once
#include <iostream>
#include <fstream>
#include "graph.cpp"
using namespace std;

struct Node {
  int id, capacityPath;
};

static void swapNode(Node *x, Node *y) {
  Node temp = *x;
  *x = *y;
  *y = temp;
}

static void swapInt(int *x, int *y) {
  int temp = *x;
  *x = *y;
  *y = temp;
}


class BinaryHeap {
private:
  const int MAX_SIZE;
  Node *heap;
  int *rotulo;
  int size;

public:
  BinaryHeap(int num);
  ~BinaryHeap();

  static int parent(int i) { return (i - 1) / 2; }
  static int leftChild(int i) { return 2 * i + 1; }
  static int rightChild(int i) { return 2 * i + 2; }

  bool empty() { return size == 0; }

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
  
BinaryHeap::BinaryHeap(int num) : MAX_SIZE(num) {
  heap = new Node[num];
  rotulo = new int[num];

  size = 0;
  for (int i = 0; i < MAX_SIZE; i++) {
    rotulo[i] = -1;
  }
}

// insere elemeneto
void BinaryHeap::insert(Node data) {
  if (size >= MAX_SIZE) {
    cout<<"The heap is full"<<endl;
    return;
  }

  if (rotulo[data.id] != -1) {
    changePriority(rotulo[data.id], data);

  } else {
    // Insere o elemento no fim do vetor heap e atualiza a posição do elemento no vetor rótulos
    heap[size] = data;
    rotulo[data.id] = size;
    size = size + 1;

    // Sobe o elemento na árvore para manter a propriedade de heap
    upHeap(size - 1);
  }
}

void BinaryHeap::upHeap(int i) {
  while (i != 0 and heap[parent(i)].capacityPath < heap[i].capacityPath) {
    swapNode(&heap[parent(i)], &heap[i]);
    swapInt(&rotulo[heap[parent(i)].id], &rotulo[heap[i].id]);
    i = parent(i);
  }
}

// moves the item at position i of array a
// into its appropriate position
void BinaryHeap::downHeap(int i){
  int left = leftChild(i);
  int right = rightChild(i);

  // encontra o maior entre 3 nós
  int largest = i;

  // checa se o filho esquerdo é maior que o nó atual
  if (left < size && heap[left].capacityPath > heap[largest].capacityPath) {
    largest = left;
  }

  // checa se o filho direito é maior que o nó atual
  if (right < size && heap[right].capacityPath > heap[largest].capacityPath) {
    largest = right;
  }

  // Troca o nó i com seu maior filho
  // e repete o processo até que o nó atual seja maior que qualquer um de seus filhos
  if (largest != i) {
    swapNode(&heap[i], &heap[largest]);
    swapInt(&rotulo[heap[i].id], &rotulo[heap[largest].id]);
    downHeap(largest);
  }
}

void BinaryHeap::changePriority(int i, Node data) {
  if (i > size) return;

  Node aux = heap[i];
  heap[i] = data;

  if (data.capacityPath > aux.capacityPath) upHeap(i);
  if (data.capacityPath < aux.capacityPath) downHeap(i);
}

Node BinaryHeap::extractMax() {
  Node maxItem = heap[0];

  // Substitui o primeiro elemento do heap pelo ultimo
  heap[0] = heap[size - 1];
  rotulo[heap[0].id] = 0;
  size = size - 1;
  rotulo[maxItem.id] = -1;

  // mantém a propriedade de heap atualizando o heap 
  downHeap(0);

  return maxItem;
}

Node BinaryHeap::getMax() {
  return heap[0];
}

int BinaryHeap::getIndexNode(int i) {
  return rotulo[i];
}

Node BinaryHeap::getNode(int rotuloNode) {
  if (rotulo[rotuloNode] != -1) {
    return heap[rotulo[rotuloNode]];
  }

  Node aux;
  aux.id = rotuloNode;
  aux.capacityPath = 0;

  return aux;
}

// prints the heap
void BinaryHeap::printHeap() {
  for (int i = 0; i < size; i++) {
    cout << "("<< i << "," << heap[i].id << ","<< heap[i].capacityPath << ") " ;
  }
  cout<<endl;
}

BinaryHeap::~BinaryHeap() {
  delete [] rotulo;
  delete [] heap;
}