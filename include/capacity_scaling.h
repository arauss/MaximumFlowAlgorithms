// Uma implementação do algoritmo Capacity Scaling
// Proposto por Ahuja e Orlin
// Tempo de pior caso: O(m^2 * log(U))

// Ideia geral: Procura o arco de maior capacidade [U = max_u(i,j)) para todo (i,j) \in Gf]
// no grafo recebido e cria um grafo residual GfΔ em função do parâmentro Δ = U que contém
// apenas o arcos com uf(i,j) = Δ.
// Enquanto existir, procura um (s-t)-caminho simples no grafo residual Δ e empurra fluxo.
// Quando não existirem mais caminhos em GfΔ, atualiza o parâmetro Δ para a metade do seu valor 
// e repete o processo de encontrar caminhos e enviar fluxo até que Δ = 1 e não haja mais (s-t)-caminhos.

#ifndef CAPACITY_SCALING
#define CAPACITY_SCALING

#include "graph.h"

int capacity_scaling_max_flow(GraphAdjacencyList &Gf, int s, int t, int *numPaths);

#endif