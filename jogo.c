#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Jogo.h"

// definicao de sequencias de escape ANSI para formatacao de cores no terminal
#define RESET    "\033[0m"
#define VERMELHO "\033[31m"
#define VERDE    "\033[32m"
#define AMARELO  "\033[33m"
#define AZUL     "\033[34m"
#define MAGENTA  "\033[35m"
#define CIANO    "\033[36m"

// recuo padrao unico para todo o projeto
#define TAB "               "

struct item {
    int codigo;
    char nome[30];
    int valor;
    char raridade[20];
};

struct jogo {
    Grafo* mapa;
    Node* raizArvore;
    Item itens[MAX_ITENS];
    int temItemLago[MAX_LAGOS];
    int lagoAtual;
    int ultimoCodigo;
};

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// arte ASCII com espacamento interno ajustado para centralizar dentro da largura padrao de 60 colunas
void mostrarLogoPesca() {
    printf(CIANO);
    printf(TAB "              __ _     _     _              \n");
    printf(TAB "             / _(_)   | |   (_)             \n");
    printf(TAB "            | |_ _ ___| |__  _ _ __   __ _  \n");
    printf(TAB "            |  _| / __| '_ \\| | '_ \\ / _` | \n");
    printf(TAB "            | | | \\__ \\ | | | | | | | (_| | \n");
    printf(TAB "            |_| |_|___/_| |_|_|_| |_|\\__, | \n");
    printf(TAB "                                      __/ | \n");
    printf(TAB "                                     |___/  \n");
    printf(RESET);
}

void mostrarPeixePescado() {
    printf(VERDE);
    printf(TAB "                                                ,///////.            ./\n");
    printf(TAB "-._                                       __,---//////////,        .///\n");
    printf(TAB "   `-._                                ,-'   ) ) ) ) ) )''////_    /////\n");
    printf(TAB "       `-._        /)             _,-' ))`. ) ) ) ) ) ) ) ) )`-.//////\n");
    printf(TAB "           `-.  ,-=([=           / ()_)))))\\\\ ) ) ) ) ) ) ) ) )////////\n");
    printf(TAB "              O/    \\)           \\____ )))))) ) ) ) ) ) ) ) ) \\\\\\\\\\\\\\\\\n");
    printf(TAB "              \\        _   Krogg   `````.)))/ ) ) ) ) ) ) ) ),-`\\\\\\\\\\\n");
    printf(TAB "               \\       )            ___,')),') ) ) ) )_),,--'    \\\\\\\\\\\n");
    printf(TAB "                \\=(o)####HHF\"     (_______.\\\\\\\\)),_--'\"            `\\\\\\\\\n");
    printf(TAB "                                           -\\\\\\\                    `\\\\\\\n");
    printf(TAB "                                             \\\\\\                    `\\\\\\\n");
    printf(RESET);
}

void mostrarLixo() {
    printf(AMARELO);
    printf(TAB "         ,|_|,   ,|_|,\n");
    printf(TAB "         |===|   |===|\n");
    printf(TAB "         |   |   |   |\n");
    printf(TAB "         /  &|   |&  \\\n");
    printf(TAB "  .-'`  , )* *( ,  `'-.\n");
    printf(TAB "  `\"\"\"\"\"`\"`   `\"`\"\"\"\"\"`\n");
    printf(RESET);
}

void exibirMenu() {
    printf("\n");
    printf(TAB AZUL "******************************************" RESET "       ,-.\n");
    printf(TAB AZUL "*" AMARELO "            JOGO DE PESCARIA            " AZUL " *" RESET "     _n_  \\ Clyde W. Watson\n");
    printf(TAB AZUL "******************************************" RESET "    (---)  `-.___________________________________\n");
    printf(TAB AZUL "* 1. " VERDE "MOVER" AZUL "    Ir para lago adjacente      *" RESET "  ~^~^~|~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~\n");
    printf(TAB AZUL "* 2. " CIANO "MAPA" AZUL "     Ver mapa dos lagos          *" RESET "       |\n");
    printf(TAB AZUL "* 3. " MAGENTA "PESCAR" AZUL "   Pescar no lago atual        *" RESET "       |                                         }-<#('c\n");
    printf(TAB AZUL "* 4. " AMARELO "INVENT" AZUL "   Ver inventario              *" RESET "       |\n");
    printf(TAB AZUL "* 5. " VERMELHO "BUSCAR" AZUL "   Buscar peixe por codigo     *" RESET "       |\n");
    printf(TAB AZUL "* 6. " VERMELHO "SOLTAR" AZUL "   Soltar peixe                *" RESET "       |                   }-<#('c                           <')\"={\n");
    printf(TAB AZUL "* 7. " CIANO "STATS" AZUL "    Estatisticas da arvore      *" RESET "       |\n");
    printf(TAB AZUL "* 8. " VERDE "INFO" AZUL "     Mostrar peixe do lago       *" RESET "       |      ><>                        <')\"={\n");
    printf(TAB AZUL "* 0. " VERMELHO "SAIR" AZUL "    Encerrar programa            *" RESET "       |                                                <><\n");
    printf(TAB AZUL "******************************************" RESET "       |             }<('>             }<('> \n");
    printf(TAB                                                   "                                             J   <')\"={\n");
    printf(TAB VERDE "\nEscolha uma opcao: " RESET);
}

Jogo* criarJogo() {
    Jogo* j = (Jogo*) malloc(sizeof(Jogo));
    if (!j) return NULL;

    j->mapa = cria_Grafo(MAX_LAGOS, MAX_LAGOS, 0);
    if (!j->mapa) {
        free(j);
        return NULL;
    }

    j->raizArvore = NULL;

    for (int i = 0; i < MAX_ITENS; i++) {
        j->itens[i].codigo = -1;
        j->itens[i].nome[0] = '\0';
        j->itens[i].valor = 0;
        j->itens[i].raridade[0] = '\0';
    }

    for (int i = 0; i < MAX_LAGOS; i++) {
        j->temItemLago[i] = -1;
    }

    j->lagoAtual = 0;
    j->ultimoCodigo = 1;
    srand(time(NULL));
    return j;
}

void destruirJogo(Jogo* j) {
    if (j) {
        if (j->mapa) libera_Grafo(j->mapa);
        if (j->raizArvore) liberarArvoreB(j->raizArvore);
        free(j);
    }
}

void adicionarConexao(Jogo* j, int lago1, int lago2, float distancia) {
    if (j == NULL) return;
    insereAresta(j->mapa, lago1, lago2, 0, distancia);
}

void adicionarPeixeLago(Jogo* j, int lago, char* nome, int valor, char* raridade) {
    if (j == NULL || lago < 0 || lago >= MAX_LAGOS) return;
    if (j->temItemLago[lago] != -1) {
        printf(TAB "Lago %d ja possui um peixe padrao.\n", lago);
        return;
    }
    int codigo = j->ultimoCodigo++;
    j->itens[codigo].codigo = codigo;
    strcpy(j->itens[codigo].nome, nome);
    j->itens[codigo].valor = valor;
    strcpy(j->itens[codigo].raridade, raridade);
    j->temItemLago[lago] = codigo;
}

int moverJogador(Jogo* j, int destino) {
    if (j == NULL) return 0;
    int atual = j->lagoAtual;
    if (existe_aresta(j->mapa, atual, destino)) {
        j->lagoAtual = destino;
        return 1;
    }
    printf("\n" TAB "[!] Nao ha caminho direto do lago %d para o lago %d.\n", atual, destino);
    return 0;
}

void mostrarLagosVizinhos(Jogo* j) {
    if (j == NULL) return;
    int atual = j->lagoAtual;
    int grau = grau_vertice(j->mapa, atual);

    printf("(");
    for (int k = 0; k < grau; k++) {
        int viz = vizinho_vertice(j->mapa, atual, k);
        printf("%d", viz);
        if (k < grau - 1) printf(", ");
    }
    printf(")\n");
}

int pescar(Jogo* j) {
    if (j == NULL) return 0;
    int lago = j->lagoAtual;
    int codPadrao = j->temItemLago[lago];

    if (codPadrao != -1) {
        if (rand() % 100 < 60) {
            j->raizArvore = inserirNo(j->raizArvore, codPadrao);
            if (j->raizArvore == NULL) {
                printf(TAB "Erro ao inserir no inventario.\n");
                return 0;
            }
            mostrarPeixePescado();
            printf(TAB "Voce pescou um %s (codigo %d, valor %d, %s)!\n",
                   j->itens[codPadrao].nome, codPadrao,
                   j->itens[codPadrao].valor, j->itens[codPadrao].raridade);
            return 1;
        }
    }

    if (rand() % 100 < 70) {
        if (rand() % 10 < 6) {

            char nome[30], raridade[20];
            int valor;
            int sorte = rand() % 100;
            if (sorte < 40)      { strcpy(nome, "Tilapia");  valor = 10; strcpy(raridade, "comum"); }
            else if (sorte < 70) { strcpy(nome, "Dourado");  valor = 30; strcpy(raridade, "raro"); }
            else if (sorte < 90) { strcpy(nome, "Pirarucu"); valor = 50; strcpy(raridade, "epico"); }
            else                 { strcpy(nome, "Tucunare"); valor = 70; strcpy(raridade, "lendario"); }

            int novoCod = j->ultimoCodigo++;
            j->itens[novoCod].codigo = novoCod;
            strcpy(j->itens[novoCod].nome, nome);
            j->itens[novoCod].valor = valor;
            strcpy(j->itens[novoCod].raridade, raridade);

            j->raizArvore = inserirNo(j->raizArvore, novoCod);
            mostrarPeixePescado();
            printf(TAB "Voce pescou um %s (codigo %d, valor %d, %s)!\n", nome, novoCod, valor, raridade);
            return 1;

        } else {
            char lixo[][20] = {"Bota velha", "Latinha amassada", "Garrafa pet", "Saco plastico", "Pneu"};
            int idx = rand() % 5;
            int valorLixo = rand() % 5 + 1;
            int novoCod = j->ultimoCodigo++;

            j->itens[novoCod].codigo = novoCod;
            strcpy(j->itens[novoCod].nome, lixo[idx]);
            j->itens[novoCod].valor = valorLixo;
            strcpy(j->itens[novoCod].raridade, "lixo");

            j->raizArvore = inserirNo(j->raizArvore, novoCod);
            mostrarLixo();
            printf(TAB "Voce pescou %s (codigo %d, valor %d)!\n", lixo[idx], valorLixo);
            return 1;
        }
    } else {
        printf(TAB "Voce nao pescou nada. Tente novamente!\n");
        return 0;
    }
}

static void callbackListar(int chave, void *ctx) {
    Jogo *j = (Jogo*) ctx;
    if (j->itens[chave].codigo != -1) {
        printf(TAB "Codigo %d: %s (Valor: %d, Raridade: %s)\n",
               chave, j->itens[chave].nome, j->itens[chave].valor, j->itens[chave].raridade);
    }
}

void listarInventario(Jogo* j) {
    if (j == NULL || j->raizArvore == NULL) {
        printf(TAB "Inventario vazio.\n");
        return;
    }
    printf("\n" TAB "--- INVENTARIO (ordem crescente de codigo) ---\n\n");
    percorrerEmOrdem(j->raizArvore, callbackListar, j);
    printf(TAB "-------------------------------------------\n");
}

void buscarItem(Jogo* j, int codigo) {
    if (j == NULL) return;
    busca(j->raizArvore, codigo);
}

void removerItem(Jogo* j, int codigo) {
    if (j == NULL) return;
    j->raizArvore = excluirNo(j->raizArvore, codigo);
}

void estatisticasArvore(Jogo* j) {
    if (j == NULL) return;
    int nos = totalNos(j->raizArvore);
    int alt = alturaArvore(j->raizArvore);
    printf(TAB "--- Estatisticas da Arvore B ---\n\n");
    printf(TAB "Total de nos: %d\n", nos);
    printf(TAB "Altura: %d\n", alt);
    printf(TAB "--------------------------------\n");
}

void imprimirMapa(Jogo* j) {
    if (j == NULL) return;

    #define LARG 50
    #define ALT 13
    char mapa[ALT][LARG];

    for (int i = 0; i < ALT; i++)
        for (int j = 0; j < LARG; j++)
            mapa[i][j] = ' ';

    int pos[10][2] = {
        {5, 0}, {5, 10}, {8, 10}, {2, 20}, {5, 20},
        {8, 20}, {11,20}, {0, 30}, {5, 30}, {8, 30}
    };

    for (int i = 0; i < MAX_LAGOS; i++) {
        int linha = pos[i][0], col = pos[i][1];
        char rotulo[20];
        if (i == j->lagoAtual)
            sprintf(rotulo, "*%d*", i);
        else
            sprintf(rotulo, "[%d]", i);

        for (int k = 0; k < (int)strlen(rotulo) && col + k < LARG; k++) {
            mapa[linha][col + k] = rotulo[k];
        }
    }

    for (int i = 0; i < MAX_LAGOS; i++) {
        int grau = grau_vertice(j->mapa, i);
        for (int k = 0; k < grau; k++) {
            int viz = vizinho_vertice(j->mapa, i, k);
            if (viz > i) {
                int l1 = pos[i][0], c1 = pos[i][1] + 3;
                int l2 = pos[viz][0], c2 = pos[viz][1];

                if (l1 == l2) {
                    if (c1 < c2) {
                        for (int c = c1; c <= c2; c++) {
                            if (mapa[l1][c] == ' ') mapa[l1][c] = '-';
                            else if (mapa[l1][c] == '|') mapa[l1][c] = '+';
                        }
                    } else {
                        for (int c = c2; c <= c1; c++) {
                            if (mapa[l1][c] == ' ') mapa[l1][c] = '-';
                            else if (mapa[l1][c] == '|') mapa[l1][c] = '+';
                        }
                    }
                } else if (c1 == c2) {
                    if (l1 < l2) {
                        for (int l = l1; l <= l2; l++) {
                            if (mapa[l][c1] == ' ') mapa[l][c1] = '|';
                            else if (mapa[l][c1] == '-') mapa[l][c1] = '+';
                        }
                    } else {
                        for (int l = l2; l <= l1; l++) {
                            if (mapa[l][c1] == ' ') mapa[l][c1] = '|';
                            else if (mapa[l][c1] == '-') mapa[l][c1] = '+';
                        }
                    }
                }
            }
        }
    }

    printf("\n" TAB "--- MAPA DOS LAGOS ---\n\n");
    for (int i = 0; i < ALT; i++) {
        printf(TAB);
        for (int j = 0; j < LARG; j++) {
            printf("%c", mapa[i][j]);
        }
        printf("\n");
    }

    printf("\n");
    for (int i = 0; i < MAX_LAGOS; i++) {
        if (j->temItemLago[i] != -1) {
            int cod = j->temItemLago[i];
            printf(TAB "Lago %d tem peixe: %s (codigo %d)\n", i, j->itens[cod].nome, cod);
        }
    }
    printf("\n" TAB "Lago atual: %d (indicado com *)\n", j->lagoAtual);
    printf(TAB "-------------------------\n");
}

int getLagoAtual(Jogo* j) {
    return j ? j->lagoAtual : -1;
}

int temPeixeNoLago(Jogo* j) {
    if (j == NULL) return 0;
    return j->temItemLago[j->lagoAtual] != -1;
}

void mostrarPeixeLago(Jogo* j) {
    if (j == NULL) return;
    int cod = j->temItemLago[j->lagoAtual];
    if (cod == -1) {
        printf(TAB "Nao ha peixe neste lago.\n");
    } else {
        printf(TAB "Peixe disponivel: %s (valor %d, %s)\n",
               j->itens[cod].nome, j->itens[cod].valor, j->itens[cod].raridade);
    }
}
