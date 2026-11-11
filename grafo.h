#ifndef GRAFO_H
#define GRAFO_H

/**
 * @brief tipo opaco representando o grafo de adjacencias dos lagos (TAD Grafo)
 */
typedef struct grafo Grafo;

// --- operacoes principais do grafo ---
Grafo* cria_Grafo(int nro_vertices, int grau_max, int eh_ponderado);
void libera_Grafo(Grafo* gr);
int insereAresta(Grafo* gr, int orig, int dest, int eh_digrafo, float peso);
int removeAresta(Grafo* gr, int orig, int dest, int eh_digrafo);
void imprime_Grafo(Grafo *gr);

// --- funcoes de encapsulamento / consulta ---
int grau_vertice(Grafo *gr, int v);
int vizinho_vertice(Grafo *gr, int v, int pos);
int existe_aresta(Grafo *gr, int v1, int v2);

#endif // GRAFO_H
