# Maximum Flow Problem

O Maximum Flow Problem consiste em determinar como enviar a maior quantidade de fluxo possível entre dois vértices
de uma rede que possui capacidades nos arcos, de modo que nenhuma capacidade seja excedida e nenhum fluxo se perca.
Detalhamos melhor o problema a seguir.

___
### Input:
+ um grafo direcionado G(V, A), sendo V o conjunto de vértices e A o conjunto de arcos com capacidade inteira positiva; 
+ um par de vértices distintos s e t.

### Restrições:
+ Capacidade: 0 <= f(i,j) <= u(i,j), para todo (i,j) pertencente a A;
+ Conservação de fluxo: somatório(f_in(i)) = somatório(f_out(i)), para todo nó i diferente de s e t.

### Objetivo:
+ Encontrar um fluxo f que maximize o valor total que sai da fonte s e chega no terminal t, respeitando as restrições acima.
___

Apresentamos neste repositório a implementação de alguns algoritmos clássicos que resolvem o problema do fluxo máximo em tempo polinomial.
