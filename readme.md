# 🧠 Jogo de Quiz em C (Console)

Um jogo de perguntas e respostas (estilo "Show do Milhão") desenvolvido inteiramente em C puro, rodando diretamente no terminal. O projeto apresenta um sistema de progressão de dificuldade, mecânicas de ajuda e carregamento dinâmico de perguntas via arquivo JSON.

## 📋 Sobre o Projeto

Este projeto foi desenvolvido para demonstrar o uso de lógica de programação avançada em C sem a dependência de bibliotecas externas complexas.

**Destaque Técnico:** O sistema inclui um **Parser JSON Manual**. Em vez de usar bibliotecas como `cJSON` ou `Jansson`, foi implementado um algoritmo próprio de leitura de arquivos que interpreta a estrutura JSON caractere por caractere, identificando chaves, valores e arrays.

## ✨ Funcionalidades

* **5 Níveis de Dificuldade:** As perguntas ficam mais difíceis conforme o jogador avança.
* **Banco de Dados Externo:** Todas as perguntas são lidas de um arquivo `perguntas.json`.
* **Sistema de Vidas:** O jogo encerra ao errar uma questão.
* **Ações Especiais (Ajudas):** O jogador possui 3 ajudas que podem ser usadas uma vez por partida:
    * ⏭️ **Pular:** Avança para o próximo nível sem responder.
    * 🔄 **Trocar:** Sorteia uma nova pergunta do mesmo nível.
    * 💡 **Dica:** Exibe uma dica de texto relacionada à resposta.
* **Interface de Console:** Menu interativo e feedback visual simples.

## 🛠️ Tecnologias Utilizadas

* **Linguagem C** (Standard Libraries: `stdio.h`, `stdlib.h`, `string.h`, `time.h`).
* **Manipulação de Arquivos (File I/O)**.
* **Alocação de Memória e Structs**.
* **Lógica de Parsing de Texto**.

## 🚀 Como Compilar e Rodar

### Pré-requisitos
Você precisa de um compilador C instalado (como GCC ou MinGW no Windows).

### Passo a Passo

1.  **Clone ou baixe este repositório.**
2.  **Compile o código:**
    Abra o terminal na pasta do arquivo `jogo.c` e execute:
    ```bash
    gcc jogo.c -o jogo
    ```
3.  **Certifique-se do arquivo JSON:**
    O arquivo `perguntas.json` **DEVE** estar na mesma pasta onde o executável (`jogo.exe` ou `./jogo`) foi criado.
4.  **Execute o jogo:**
    * **Windows:**
        ```cmd
        jogo.exe
        ```
    * **Linux/Mac:**
        ```bash
        ./jogo
        ```
