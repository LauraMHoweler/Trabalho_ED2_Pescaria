#include <stdio.h>
#include <stdlib.h>
#include "Jogo.h"

// recuo padrao unico para todo o projeto
#define TAB "               "

int main() {
    // inicializacao e alocacao do estado principal do jogo (grafo, arvore B, inventario)
    Jogo* jogo = criarJogo();
    if (jogo == NULL) {
        printf(TAB "Erro ao criar jogo.\n");
        return 1;
    }

    // configuracao da topologia do mapa: conecta os lagos criando caminhos (arestas do grafo)
    adicionarConexao(jogo, 0, 1, 1);
    adicionarConexao(jogo, 0, 2, 1);
    adicionarConexao(jogo, 1, 3, 1);
    adicionarConexao(jogo, 1, 4, 1);
    adicionarConexao(jogo, 2, 5, 1);
    adicionarConexao(jogo, 2, 6, 1);
    adicionarConexao(jogo, 3, 7, 1);
    adicionarConexao(jogo, 4, 8, 1);
    adicionarConexao(jogo, 5, 9, 1);

    // cadastra os peixes fixos/padroes em lagos especificos
    adicionarPeixeLago(jogo, 1, "Tilapia", 10, "comum");
    adicionarPeixeLago(jogo, 3, "Dourado", 30, "raro");
    adicionarPeixeLago(jogo, 5, "Pirarucu", 50, "epico");
    adicionarPeixeLago(jogo, 7, "Tucunare", 70, "lendario");
    adicionarPeixeLago(jogo, 9, "Salmao", 40, "raro");

    int opcao, destino, codigo;

    // tela de boas-vindas inicial padronizada em 60 colunas
    limparTela();
    mostrarLogoPesca();

    printf("\n" TAB "============================================================\n");
    printf(TAB "               BEM-VINDO AO JOGO DE PESCARIA!               \n");
    printf(TAB "============================================================\n\n");
    printf(TAB "Voce inicia sua jornada no Lago 0.\n");
    printf(TAB "Dica: Use a opcao 2 (MAPA) para visualizar as conexoes.\n\n");
    printf(TAB "------------------------------------------------------------\n");
    printf(TAB "Pressione Enter para comecar...");

    getchar();
    limparTela();

    // loop principal de interacao com o usuario (menu interativo)
    do {
        limparTela();
        mostrarLogoPesca();
        exibirMenu();

        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                printf("\n" TAB "(1) MOVER DE LAGO\n\n");
                printf(TAB "Lago atual: %d\n", getLagoAtual(jogo));

                printf(TAB "Lagos vizinhos conectados: ");
                mostrarLagosVizinhos(jogo);

                printf("\n" TAB "Digite o numero do lago para onde deseja ir: ");
                scanf("%d", &destino);
                limparBuffer();

                if (moverJogador(jogo, destino)) {
                    printf("\n" TAB "[SUCESSO] Voce caminhou ate o Lago %d!\n\n", getLagoAtual(jogo));

                    if (temPeixeNoLago(jogo)) {
                        printf(TAB "(i) Este lago possui uma especie nativa fixa registrada:\n");
                        mostrarPeixeLago(jogo);
                    } else {
                        printf(TAB "(i) Este lago nao tem peixe fixo, mas voce ainda pode pescar especies aleatorias!\n");
                    }
                }
                printf("\n" TAB "Pressione Enter para voltar ao menu...");
                getchar();
                break;

            case 2:
                printf("\n" TAB "(2) MAPA DOS LAGOS\n\n");
                imprimirMapa(jogo);
                printf("\n" TAB "Pressione Enter para voltar ao menu...");
                getchar();
                break;

            case 3:
                printf("\n" TAB "(3) PESCARIA EM ANDAMENTO...\n\n");
                printf(TAB "Jogando a linha no Lago %d...\n\n", getLagoAtual(jogo));

                if (pescar(jogo)) {
                    // a arte ASCII e detalhes do item sao exibidos internamente na funcao pescar()
                } else {
                    printf("\n" TAB "(3) Nenhum item fisgou a linha desta vez.\n");
                }
                printf("\n" TAB "Pressione Enter para voltar ao menu...");
                getchar();
                break;

            case 4:
                printf("\n" TAB "(4) MEU INVENTARIO\n\n");
                listarInventario(jogo);
                printf("\n" TAB "Pressione Enter para voltar ao menu...");
                getchar();
                break;

            case 5:
                printf("\n" TAB "(5) BUSCA NO INVENTARIO (ARVORE B)\n\n");
                printf(TAB "Digite o codigo do item/peixe que deseja consultar: ");
                scanf("%d", &codigo);
                limparBuffer();
                printf("\n" TAB "--- Pesquisando registro ---\n\n");
                buscarItem(jogo, codigo);
                printf("\n" TAB "Pressione Enter para voltar ao menu...");
                getchar();
                break;

            case 6:
                printf("\n" TAB "(6) SOLTAR / REMOVER ITEM\n\n");
                printf(TAB "Digite o codigo do peixe/item que deseja devolver ao lago: ");
                scanf("%d", &codigo);
                limparBuffer();
                printf("\n" TAB "--- Processando remocao ---\n\n");
                removerItem(jogo, codigo);
                printf("\n" TAB "Pressione Enter para voltar ao menu...");
                getchar();
                break;

            case 7:
                printf("\n" TAB "(7) ESTATISTICAS DA ARVORE B\n\n");
                estatisticasArvore(jogo);
                printf("\n" TAB "Pressione Enter para voltar ao menu...");
                getchar();
                break;

            case 8:
                printf("\n" TAB "(8) INFORMACOES DO LAGO ATUAL\n\n");
                printf(TAB "Exibindo dados do Lago %d:\n", getLagoAtual(jogo));
                mostrarPeixeLago(jogo);
                printf("\n" TAB "Pressione Enter para voltar ao menu...");
                getchar();
                break;

            case 0:
                printf("\n" TAB "(0) ENCERRANDO A APLICACAO\n\n");
                printf(TAB "Limpando estruturas de memoria e encerrando o jogo...\n");
                destruirJogo(jogo);
                printf("\n" TAB "Obrigado por jogar! Ate a proxima.\n");
                break;

            default:
                printf("\n" TAB "[!] Opcao invalida (%d). Escolha um numero de 0 a 8 do menu.\n", opcao);
                printf("\n" TAB "Pressione Enter para tentar novamente...");
                getchar();
        }

    } while (opcao != 0);

    return 0;
}
