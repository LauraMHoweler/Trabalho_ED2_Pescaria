#ifndef ARVOREB_H_INCLUDED
#define ARVOREB_H_INCLUDED

// ordem da arvore B (numero maximo de filhos por no = M)
#define M 3

/**
 * @brief tipos opacos de dados da arvore B (TAD Arvore B)
 */
typedef struct node Node;
typedef enum statusChave StatusChave;

// --- funcoes principais da arvore B ---
Node * inserirNo(Node *raiz, int chave);
StatusChave ins(Node *ptr, int chave, int *chaveIni, Node **novoNo);
void busca(Node *raiz, int chave);
int buscaChave(Node *raiz, int chave, int *chaves_arr, int n);
Node* excluirNo(Node *raiz, int chave);
StatusChave del(Node *raiz, Node *ptr, int chave);
void imprime_arvore(Node *ptr, int nivel);
void imprime_no(Node *ptr);
void liberarArvoreB(Node *raiz); // funcao para desalocar toda a arvore recursivamente

// --- funcoes de acesso e percurso ---
void percorrerEmOrdem(Node *raiz, void (*callback)(int chave, void *ctx), void *ctx);
int totalNos(Node *raiz);
int alturaArvore(Node *raiz);

#endif // ARVOREB_H_INCLUDED
