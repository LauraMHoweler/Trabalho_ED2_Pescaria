#ifndef JOGO_H
#define JOGO_H

#include "Grafo.h"
#include "ArvoreB.h"

#define MAX_LAGOS 10
#define MAX_ITENS 100

/**
 * @brief tipos opacos de dados (encapsulamento do TAD)
 * as definicoes das structs 'item' e 'jogo' ficam ocultas no arquivo Jogo.c
 */
typedef struct item Item;
typedef struct jogo Jogo;

// --- gerenciamento da instancia principal do jogo ---
Jogo* criarJogo();
void destruirJogo(Jogo* j);

// --- manipulacao do grafo (mapa de lagos) ---
void adicionarConexao(Jogo* j, int lago1, int lago2, float distancia);
void adicionarPeixeLago(Jogo* j, int lago, char* nome, int valor, char* raridade);
int moverJogador(Jogo* j, int destino);
void imprimirMapa(Jogo* j);
int getLagoAtual(Jogo* j);
int temPeixeNoLago(Jogo* j);
void mostrarPeixeLago(Jogo* j);
void mostrarLagosVizinhos(Jogo* j);

// --- manipulacao da arvore B (inventario do jogador) ---
int pescar(Jogo* j);
void listarInventario(Jogo* j);
void buscarItem(Jogo* j, int codigo);
void removerItem(Jogo* j, int codigo);
void estatisticasArvore(Jogo* j);

// --- funcoes utilitarias e de interface (terminal/ASCII) ---
void limparBuffer();
void limparTela();
void mostrarLogoPesca();
void exibirMenu();

#endif // JOGO_H
