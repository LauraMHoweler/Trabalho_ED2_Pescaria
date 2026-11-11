#include <stdio.h>
#include <stdlib.h>
#include "Grafo.h"

#define TAB "                    "

/**
 * @brief estrutura concreta do grafo (matriz dinamica de adjacencias)
 */
struct grafo {
    int eh_ponderado;   // flag para indicar se as arestas possuem peso
    int nro_vertices;   // quantidade total de vertices (lagos)
    int grau_max;       // maximo de conexoes que um lago pode ter
    int** arestas;      // matriz dinamicamente alocada: arestas[vertice][posicao]
    float** pesos;      // matriz dinamicamente alocada para guardar os pesos das arestas
    int* grau;          // array dinamico registrando o grau (numero de conexoes ativas) de cada vertice
};

Grafo* cria_Grafo(int nro_vertices, int grau_max, int eh_ponderado) {
    Grafo *gr = (Grafo*) malloc(sizeof(struct grafo));
    if (gr != NULL) {
        int i;
        gr->nro_vertices = nro_vertices;
        gr->grau_max = grau_max;
        gr->eh_ponderado = (eh_ponderado != 0) ? 1 : 0;

        // aloca os vetores principais de grau e de ponteiros para arestas
        gr->grau = (int*) calloc(nro_vertices, sizeof(int));
        gr->arestas = (int**) malloc(nro_vertices * sizeof(int*));

        // validacao de alocacao de memoria segura
        if (!gr->grau || !gr->arestas) {
            free(gr->grau);
            free(gr->arestas);
            free(gr);
            return NULL;
        }

        // aloca o array de conexoes de cada vertice
        for (i = 0; i < nro_vertices; i++) {
            gr->arestas[i] = (int*) malloc(grau_max * sizeof(int));
        }

        // aloca matriz de pesos se o grafo for ponderado
        if (gr->eh_ponderado) {
            gr->pesos = (float**) malloc(nro_vertices * sizeof(float*));
            if (gr->pesos) {
                for (i = 0; i < nro_vertices; i++)
                    gr->pesos[i] = (float*) malloc(grau_max * sizeof(float));
            }
        } else {
            gr->pesos = NULL;
        }
    }
    return gr;
}

void libera_Grafo(Grafo* gr) {
    if (gr != NULL) {
        int i;
        // libera cada linha da matriz de arestas e o ponteiro principal
        if (gr->arestas) {
            for (i = 0; i < gr->nro_vertices; i++)
                free(gr->arestas[i]);
            free(gr->arestas);
        }
        // libera a matriz de pesos se existir
        if (gr->eh_ponderado && gr->pesos) {
            for (i = 0; i < gr->nro_vertices; i++)
                free(gr->pesos[i]);
            free(gr->pesos);
        }
        // libera o vetor de graus e o ponteiro da struct principal
        if (gr->grau) free(gr->grau);
        free(gr);
    }
}

int insereAresta(Grafo* gr, int orig, int dest, int eh_digrafo, float peso) {
    if (gr == NULL) return 0;
    if (orig < 0 || orig >= gr->nro_vertices) return 0;
    if (dest < 0 || dest >= gr->nro_vertices) return 0;
    if (gr->grau[orig] >= gr->grau_max) return 0; // previne estouro de limite do array

    // adiciona o destino na lista do vertice de origem
    gr->arestas[orig][gr->grau[orig]] = dest;
    if (gr->eh_ponderado && gr->pesos)
        gr->pesos[orig][gr->grau[orig]] = peso;
    gr->grau[orig]++;

    // se o grafo nao for direcionado (digrafo=0), insere a aresta de volta (destino -> origem)
    if (eh_digrafo == 0)
        insereAresta(gr, dest, orig, 1, peso);
    return 1;
}

int removeAresta(Grafo* gr, int orig, int dest, int eh_digrafo) {
    if (gr == NULL) return 0;
    if (orig < 0 || orig >= gr->nro_vertices) return 0;
    if (dest < 0 || dest >= gr->nro_vertices) return 0;

    int i = 0;
    while (i < gr->grau[orig] && gr->arestas[orig][i] != dest) i++;
    if (i == gr->grau[orig]) return 0; // aresta nao encontrada

    gr->grau[orig]--;
    // substitui a aresta removida pela ultima aresta do array para manter o vetor compacto
    gr->arestas[orig][i] = gr->arestas[orig][gr->grau[orig]];
    if (gr->eh_ponderado && gr->pesos)
        gr->pesos[orig][i] = gr->pesos[orig][gr->grau[orig]];

    if (eh_digrafo == 0)
        removeAresta(gr, dest, orig, 1);
    return 1;
}

void imprime_Grafo(Grafo *gr) {
    if (gr == NULL) return;
    int i, j;
    for (i = 0; i < gr->nro_vertices; i++) {
        printf(TAB ".Vertice %d, vinculado a: ", i);
        for (j = 0; j < gr->grau[i]; j++) {
            if (gr->eh_ponderado && gr->pesos)
                printf("%d(Peso %.2f), ", gr->arestas[i][j], gr->pesos[i][j]);
            else
                printf("%d, ", gr->arestas[i][j]);
        }
        printf("\n");
    }
}

// --- funcoes de acesso para respeitar o encapsulamento ---

int grau_vertice(Grafo *gr, int v) {
    if (gr == NULL || v < 0 || v >= gr->nro_vertices) return -1;
    return gr->grau[v];
}

int vizinho_vertice(Grafo *gr, int v, int pos) {
    if (gr == NULL || v < 0 || v >= gr->nro_vertices) return -1;
    if (pos < 0 || pos >= gr->grau[v]) return -1;
    return gr->arestas[v][pos];
}

int existe_aresta(Grafo *gr, int v1, int v2) {
    if (gr == NULL) return 0;
    if (v1 < 0 || v1 >= gr->nro_vertices) return 0;
    if (v2 < 0 || v2 >= gr->nro_vertices) return 0;
    for (int i = 0; i < gr->grau[v1]; i++) {
        if (gr->arestas[v1][i] == v2) return 1;
    }
    return 0;
}
