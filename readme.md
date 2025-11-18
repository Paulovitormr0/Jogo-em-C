# 🧠 Jogo de Quiz em C (Console)

Um jogo de perguntas e respostas (estilo "Show do Milhão") desenvolvido inteiramente em **C puro**, rodando diretamente no terminal.

O projeto demonstra a capacidade de manipular estruturas (`struct`) e realizar **parsing manual de arquivos JSON** sem o uso de bibliotecas externas (`cJSON`, `Jansson`, etc.).

## ✨ Funcionalidades

* **Progressão de Nível:** 5 níveis de dificuldade.
* **Banco de Perguntas:** Carregamento dinâmico de perguntas via `perguntas.json`.
* **Ações Especiais (Ajudas):**
    * ⏭️ **Pular:** Avança para o próximo nível.
    * 🔄 **Trocar:** Sorteia uma nova pergunta do nível atual.
    * 💡 **Dica:** Exibe a dica associada à pergunta.
* **Controle de Fluxo:** Utilização de `goto` para gerenciar saltos no menu de ações (`RECARREGAR_PERGUNTA`, `EXIBIR_PERGUNTA`).

***

## 🛠️ 1. Instruções para Compilação (GCC/MinGW)

Para rodar o jogo, você precisa ter o compilador GCC/MinGW instalado e configurado no seu sistema (como visto nos seus *screenshots*).

1.  **Abra o terminal** na pasta que contém o arquivo `jogo.c`.
2.  **Execute o comando de compilação** para criar o executável:
    ```bash
    gcc jogo.c -o jogo
    ```
    *(O comando cria um arquivo chamado `jogo.exe` no Windows ou um executável chamado `jogo` no Linux/Mac).*

***

## ▶️ 2. Uso do Executável

### 2.1 Configuração de Arquivo (CRUCIAL)

O arquivo de perguntas (`perguntas.json`) **DEVE** estar na **mesma pasta** onde o arquivo `jogo.exe` está localizado.

Se você usa VS Code e o executável está na pasta `output`, o arquivo JSON também deve estar lá.

### 2.2 Execução

Após a compilação, execute o programa:

* **Windows:**
    ```cmd
    jogo.exe
    ```
* **Linux/Mac:**
    ```bash
    ./jogo
    ```

### 2.3 Navegação

O jogo apresentará um loop de ações:

| Opção | Ação |
| :---: | :--- |
| **1** | **Responder pergunta** |
| **2** | **Sair do jogo** (Desistir) |
| **3** | **Utilizar ajuda** (Pular, Trocar, Dica) |
| **4** | **Mostrar estado** (Nível e Ajudas) |
| **5** | **Atualizar tela** (Repete a pergunta e o menu) |

***

## 📝 3. Estrutura do Arquivo JSON (`perguntas.json`)

Seu código possui um parser manual, que exige que os campos dentro de cada pergunta (`{...}`) estejam em uma ordem específica.

### Ordem e Tipos de Dados

O arquivo deve ser um **Array** `[]` contendo objetos `{}`. Cada objeto deve seguir rigorosamente a ordem abaixo:

| Campo JSON | Tipo | Propósito | Exemplo |
| :---: | :---: | :--- | :--- |
| **`"texto"`** | String | O enunciado da pergunta. | `"Qual é a fórmula da água?"` |
| **`"alternativas"`**| Array de Strings | As 4 opções de resposta. | `["O2", "H2", "H2O", "C"]` |
| **`"resposta"`** | Número | O número da opção correta (1, 2, 3 ou 4). | `3` |
| **`"dica"`** | String | O texto da dica para a ação especial. | `"Possui 2 hidrogênios."` |

### Exemplo Completo

```json
[
  {
    "texto": "Quem desenvolveu a Teoria da Relatividade?",
    "alternativas": ["Newton", "Einstein", "Tesla", "Galileu"],
    "resposta": 2,
    "dica": "Famoso pela equação E=mc²."
  },
  {
    "texto": "Qual a linguagem mais falada no mundo?",
    "alternativas": ["Inglês", "Espanhol", "Hindi", "Mandarim"],
    "resposta": 4,
    "dica": "É a principal língua da China."
  }
  // ... continue para ter um total de 25 objetos
]
