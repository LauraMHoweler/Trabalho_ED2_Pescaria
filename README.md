# 🎣 Jogo de Pescaria — Estruturas de Dados 2 (ED43S)

Trabalho prático desenvolvido para a disciplina de **Estruturas de Dados 2 (ED43S)** do curso de **Análise e Desenvolvimento de Sistemas (UTFPR)**.

* **Instituição:** Universidade Tecnológica Federal do Paraná (UTFPR)
* **Curso:** Análise e Desenvolvimento de Sistemas
* **Disciplina:** Estruturas de Dados 2 (ED43S)
* **Professora:**  Rubia Eliza De Oliveira Schultz Ascari
* **Autora:** Laura Maria Howeler

---

## 📌 Visão Geral do Sistema

O projeto é um jogo de pescaria simulado via terminal interativo, construído em **C** com interface estilizada em artes ASCII e cores ANSI. O objetivo principal é demonstrar a aplicação prática de duas estruturas de dados fundamentais: **Grafos** e **Árvores B**.

O jogador explora uma rede de lagos interconectados, pesca diferentes espécies de peixes ou itens de valor/raridade variados, e gerencia seu inventário pessoal de capturas.

---

## 🛠️ Aspectos Técnicos e Estruturas de Dados

### 1. Mapeamento dos Lagos (Grafos)
* **Estrutura:** Grafo não-direcionado implementado via TAD modular (`Grafo.h` / `Grafo.c`).
* **Aplicação:** Os **vértices** representam os lagos e as **arestas** representam os caminhos navegáveis entre eles.
* **Navegação:** A movimentação valida a existência de arestas diretas a partir do lago atual antes de atualizar a posição do jogador.

### 2. Gerenciamento do Inventário (Árvores B)
* **Estrutura:** Árvore B de ordem $M = 3$ (`ArvoreB.h` / `ArvoreB.c`).
* **Aplicação:** Armazenamento ordenado e indexado de cada peixe ou item capturado, utilizando o `código` único como chave.
* **Operações Suportadas:**
  * **Inserção (`inserirNo`):** Mantém o inventário ordenado automaticamente e trata o transbordo via *split* dos nós.
  * **Busca (`busca`):** Localização em tempo logarítmico dos itens no inventário por código.
  * **Remoção (`excluirNo`):** Permite soltar peixes ou descartar itens com rebalanceamento dos nós (*fusão e redistribuição*).
  * **Caminhamento Em-Ordem:** Exibição do inventário em ordem crescente de código.

---

## 🖥️ Como Executar

1. Abra o arquivo `Trabalho_LauraMHoweler_ED2.cbp` no **Code::Blocks**.
2. Compile e execute o projeto pressionando **F9** (ou via menu *Build > Build and run*).
3. Utilize o menu interativo para navegar e pescar pelos lagos.
