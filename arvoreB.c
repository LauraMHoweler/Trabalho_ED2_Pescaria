#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ArvoreB.h"

#define TAB "                    "

/**
 * @brief no concreto da arvore B
 * suporta ate (M-1) chaves e M ponteiros para filhos
 */
struct node {
    int n;                 // numero atual de chaves armazenadas neste no
    int chaves[M - 1];     // vetor de chaves ordenadas
    struct node *p[M];     // ponteiros para os nos filhos
};

/**
 * @brief codigos de status retornados pelas operacoes auxiliares de insercao/remocao
 */
enum statusChave {
    Duplicado,
    NaoEncontrado,
    Sucesso,
    Inseriu,
    PoucasChaves
};

// ============================================================================
// ===== OPERAÇÕES DA ÁRVORE B ================================================
// ============================================================================

Node* inserirNo(Node *raiz, int chave) {
    Node *novoNo;
    int chaveIni;
    StatusChave status;

    status = ins(raiz, chave, &chaveIni, &novoNo);

    if (status == Duplicado)
        printf(TAB "Chave ja inserida.\n");

    // caso ocorra o "split" (divisao) da raiz antiga, uma nova raiz e criada
    if (status == Inseriu) {
        Node *raizIni = raiz;
        raiz = (Node*)malloc(sizeof(Node));
        if (raiz == NULL) return raizIni;

        raiz->n = 1;
        raiz->chaves[0] = chaveIni;
        raiz->p[0] = raizIni;
        raiz->p[1] = novoNo;
    }
    return raiz;
}

StatusChave ins(Node *ptr, int chave, int *chaveIni, Node **novoNo) {
    Node *novoPtr, *ultPtr;
    int pos, i, n, splitPos;
    int novaChave, ultChave;
    StatusChave status;

    if (ptr == NULL) {
        *novoNo = NULL;
        *chaveIni = chave;
        return Inseriu;
    }

    n = ptr->n;
    pos = buscaChave(ptr, chave, ptr->chaves, n);

    if (pos < n && chave == ptr->chaves[pos])
        return Duplicado;

    status = ins(ptr->p[pos], chave, &novaChave, &novoPtr);
    if (status != Inseriu)
        return status;

    // se o no possui espaco (menos de M-1 chaves), insere diretamente
    if (n < M - 1) {
        pos = buscaChave(ptr, novaChave, ptr->chaves, n);
        for (i = n; i > pos; i--) {
            ptr->chaves[i] = ptr->chaves[i - 1];
            ptr->p[i + 1] = ptr->p[i];
        }
        ptr->chaves[pos] = novaChave;
        ptr->p[pos + 1] = novoPtr;
        ++ptr->n;
        return Sucesso;
    }

    // caso o no esteja cheio, executa a divisao (split) do no
    if (pos == M - 1) {
        ultChave = novaChave;
        ultPtr = novoPtr;
    } else {
        ultChave = ptr->chaves[M - 2];
        ultPtr = ptr->p[M - 1];
        for (i = M - 2; i > pos; i--) {
            ptr->chaves[i] = ptr->chaves[i - 1];
            ptr->p[i + 1] = ptr->p[i];
        }
        ptr->chaves[pos] = novaChave;
        ptr->p[pos + 1] = novoPtr;
    }

    splitPos = (M - 1) / 2;
    (*chaveIni) = ptr->chaves[splitPos];
    (*novoNo) = (Node*)malloc(sizeof(Node));
    if (*novoNo == NULL) return NaoEncontrado;

    ptr->n = splitPos;
    (*novoNo)->n = M - 1 - splitPos;

    for (i = 0; i < (*novoNo)->n; i++) {
        (*novoNo)->p[i] = ptr->p[i + splitPos + 1];
        if (i < (*novoNo)->n - 1)
            (*novoNo)->chaves[i] = ptr->chaves[i + splitPos + 1];
        else
            (*novoNo)->chaves[i] = ultChave;
    }
    (*novoNo)->p[(*novoNo)->n] = ultPtr;
    return Inseriu;
}

void busca(Node *raiz, int chave) {
    int pos, i, n;
    Node *ptr = raiz;
    printf(TAB "Caminho de busca:\n" TAB);
    while (ptr) {
        n = ptr->n;
        for (i = 0; i < ptr->n; i++)
            printf(" %d", ptr->chaves[i]);
        printf("\n");

        pos = buscaChave(raiz, chave, ptr->chaves, n);
        if (pos < n && chave == ptr->chaves[pos]) {
            printf(TAB "Chave %d encontrada na posicao %d do ultimo no apresentado.\n", chave, pos);
            return;
        }
        ptr = ptr->p[pos];
        if (ptr) printf(TAB);
    }
    printf(TAB "Chave %d nao encontrada.\n", chave);
}

int buscaChave(Node *raiz, int chave, int *chaves_arr, int n) {
    int pos = 0;
    while (pos < n && chave > chaves_arr[pos])
        pos++;
    return pos;
}

Node* excluirNo(Node *raiz, int chave) {
    Node *raizIni;
    StatusChave status;
    status = del(raiz, raiz, chave);
    switch (status) {
        case NaoEncontrado:
            printf(TAB "Chave %d nao encontrada.\n", chave);
            break;
        case PoucasChaves:
            // ajusta a raiz caso a raiz anterior tenha ficado vazia
            raizIni = raiz;
            raiz = raiz->p[0];
            free(raizIni);
            break;
        default:
            return raiz;
    }
    return raiz;
}

StatusChave del(Node *raiz, Node *ptr, int chave) {
    int pos, i, pivot, n, min;
    int *chaves_arr;
    StatusChave status;
    Node **p, *esq_ptr, *dir_ptr;

    if (ptr == NULL)
        return NaoEncontrado;

    n = ptr->n;
    chaves_arr = ptr->chaves;
    p = ptr->p;
    min = (M - 1) / 2;
    pos = buscaChave(raiz, chave, chaves_arr, n);

    // se o no for folha
    if (p[0] == NULL) {
        if (pos == n || chave < chaves_arr[pos])
            return NaoEncontrado;
        for (i = pos + 1; i < n; i++) {
            chaves_arr[i - 1] = chaves_arr[i];
            p[i] = p[i + 1];
        }
        return --ptr->n >= (ptr == raiz ? 1 : min) ? Sucesso : PoucasChaves;
    }

    // se a chave a ser removida esta em um no interno
    if (pos < n && chave == chaves_arr[pos]) {
        Node *qp = p[pos], *qp1;
        int nkey;
        while (1) {
            nkey = qp->n;
            qp1 = qp->p[nkey];
            if (qp1 == NULL)
                break;
            qp = qp1;
        }
        chaves_arr[pos] = qp->chaves[nkey - 1];
        qp->chaves[nkey - 1] = chave;
    }

    status = del(raiz, p[pos], chave);
    if (status != PoucasChaves)
        return status;

    // trata o caso em que um filho fica com menos chaves que o minimo permitido (redistribuicao/fusao)
    if (pos > 0 && p[pos - 1]->n > min) {
        pivot = pos - 1;
        esq_ptr = p[pivot];
        dir_ptr = p[pos];
        dir_ptr->p[dir_ptr->n + 1] = dir_ptr->p[dir_ptr->n];
        for (i = dir_ptr->n; i > 0; i--) {
            dir_ptr->chaves[i] = dir_ptr->chaves[i - 1];
            dir_ptr->p[i] = dir_ptr->p[i - 1];
        }
        dir_ptr->n++;
        dir_ptr->chaves[0] = chaves_arr[pivot];
        dir_ptr->p[0] = esq_ptr->p[esq_ptr->n];
        chaves_arr[pivot] = esq_ptr->chaves[--esq_ptr->n];
        return Sucesso;
    }

    if (pos < n && p[pos + 1]->n > min) {
        pivot = pos;
        esq_ptr = p[pivot];
        dir_ptr = p[pivot + 1];
        esq_ptr->chaves[esq_ptr->n] = chaves_arr[pivot];
        esq_ptr->p[esq_ptr->n + 1] = dir_ptr->p[0];
        chaves_arr[pivot] = dir_ptr->chaves[0];
        esq_ptr->n++;
        dir_ptr->n--;
        for (i = 0; i < dir_ptr->n; i++) {
            dir_ptr->chaves[i] = dir_ptr->chaves[i + 1];
            dir_ptr->p[i] = dir_ptr->p[i + 1];
        }
        dir_ptr->p[dir_ptr->n] = dir_ptr->p[dir_ptr->n + 1];
        return Sucesso;
    }

    if (pos == n)
        pivot = pos - 1;
    else
        pivot = pos;

    esq_ptr = p[pivot];
    dir_ptr = p[pivot + 1];
    esq_ptr->chaves[esq_ptr->n] = chaves_arr[pivot];
    esq_ptr->p[esq_ptr->n + 1] = dir_ptr->p[0];

    for (i = 0; i < dir_ptr->n; i++) {
        esq_ptr->chaves[esq_ptr->n + 1 + i] = dir_ptr->chaves[i];
        esq_ptr->p[esq_ptr->n + 2 + i] = dir_ptr->p[i + 1];
    }
    esq_ptr->n = esq_ptr->n + dir_ptr->n + 1;
    free(dir_ptr);

    for (i = pos + 1; i < n; i++) {
        chaves_arr[i - 1] = chaves_arr[i];
        p[i] = p[i + 1];
    }
    return --ptr->n >= (ptr == raiz ? 1 : min) ? Sucesso : PoucasChaves;
}

void imprime_arvore(Node *ptr, int nivel) {
    if (ptr != NULL) {
        char indent[10] = "";
        for (int i = 0; i < nivel; i++)
            strcat(indent, "\t");
        for (int i = ptr->n; i > 0; i--) {
            imprime_arvore(ptr->p[i], nivel + 1);
            printf(TAB "%s", indent);
            printf("%5d\n", ptr->chaves[i - 1]);
        }
        imprime_arvore(ptr->p[0], nivel + 1);
    }
}

void imprime_no(Node *ptr) {
    if (ptr != NULL) {
        printf(TAB);
        for (int i = 0; i < ptr->n; i++)
            printf("%5d", ptr->chaves[i]);
        printf("\n");
    }
}

/**
 * @brief libera recursivamente a memoria de todos os nos da arvore B
 */
void liberarArvoreB(Node *raiz) {
    if (raiz == NULL) return;
    for (int i = 0; i <= raiz->n; i++) {
        liberarArvoreB(raiz->p[i]);
    }
    free(raiz);
}

// ============================================================================
// ===== FUNÇÕES DE ACESSO E ENCAPSULAMENTO ==================================
// ============================================================================

void percorrerEmOrdem(Node *raiz, void (*callback)(int chave, void *ctx), void *ctx) {
    if (raiz == NULL) return;
    int i;
    for (i = 0; i < raiz->n; i++) {
        percorrerEmOrdem(raiz->p[i], callback, ctx);
        callback(raiz->chaves[i], ctx);
    }
    percorrerEmOrdem(raiz->p[i], callback, ctx);
}

int totalNos(Node *raiz) {
    if (raiz == NULL) return 0;
    int total = 1;
    for (int i = 0; i <= raiz->n; i++)
        total += totalNos(raiz->p[i]);
    return total;
}

int alturaArvore(Node *raiz) {
    if (raiz == NULL) return 0;
    int maxAlt = 0;
    for (int i = 0; i <= raiz->n; i++) {
        int h = alturaArvore(raiz->p[i]);
        if (h > maxAlt) maxAlt = h;
    }
    return maxAlt + 1;
}
