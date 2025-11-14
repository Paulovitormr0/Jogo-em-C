#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define LINHAS_POR_PERGUNTA 8 // Pergunta + 4x Alternativas + Resposta + DICA + Linha em branco
#define MAX_LINHA 256
#define TOTAL_NIVEIS 5
void lerArquivo(const char *nomeArquivo) {
    FILE *arquivo;
    char caractere;

    arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro: Não foi possível abrir o arquivo %s\n", nomeArquivo);
        return;
    }

    printf("\n--- Imprimindo lentamente '%s' ---\n", nomeArquivo);
    while ((caractere = fgetc(arquivo)) != EOF) {
        putchar(caractere);
        fflush(stdout);     // Garante impressão imediata
    }
    printf("\n--- Fim da impressao ---\n");
    fclose(arquivo);
}

typedef struct {
    char texto[MAX_LINHA];
    char alternativas[4][MAX_LINHA];
    char dica[MAX_LINHA]; // NOVO: Campo para a dica
    int resposta_correta; // Guardamos como número
} Pergunta;

// Função auxiliar para remover o '\n' que o fgets() deixa no final
void trim_newline(char *str) {
    int len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

int buscarPerguntaNoTxt(int numero_pergunta, Pergunta *p) {
    FILE *fp = fopen("perguntas.txt", "r");
    if (fp == NULL) {
        printf("ERRO: Nao foi possivel abrir o 'perguntas.txt'\n");
        return 1;
    }

    // 1. Pular para a pergunta correta
    // Ex: para pegar a pergunta 5 (índice 4), pulamos 4 * 8 = 32 linhas
    int linhas_para_pular = numero_pergunta * LINHAS_POR_PERGUNTA;
    char buffer_lixo[MAX_LINHA];
    
    for (int i = 0; i < linhas_para_pular; i++) {
        if (fgets(buffer_lixo, MAX_LINHA, fp) == NULL) {
            printf("ERRO: Arquivo terminou antes de achar a pergunta %d\n", numero_pergunta);
            fclose(fp);
            return 1; // Erro, arquivo inesperado
        }
    }

    // 2. Ler os dados da pergunta
    if (fgets(p->texto, MAX_LINHA, fp) == NULL) return 1;           // Linha 1
    if (fgets(p->alternativas[0], MAX_LINHA, fp) == NULL) return 1; // Linha 2
    if (fgets(p->alternativas[1], MAX_LINHA, fp) == NULL) return 1; // Linha 3
    if (fgets(p->alternativas[2], MAX_LINHA, fp) == NULL) return 1; // Linha 4
    if (fgets(p->alternativas[3], MAX_LINHA, fp) == NULL) return 1; // Linha 5
    
    // 3. Ler a resposta (como texto) e converter para número
    char buffer_resposta[MAX_LINHA];
    if (fgets(buffer_resposta, MAX_LINHA, fp) == NULL) return 1; // Linha 6

    // 4. NOVO: Ler a Dica
    if (fgets(p->dica, MAX_LINHA, fp) == NULL) return 1; // Linha 7
    // A Linha 8 (em branco) é "pulada" automaticamente na próxima iteração do 'for'

    // 5. Limpar o '\n' do final de cada linha lida
    trim_newline(p->texto);
    trim_newline(p->alternativas[0]);
    trim_newline(p->alternativas[1]);
    trim_newline(p->alternativas[2]);
    trim_newline(p->alternativas[3]);
    trim_newline(p->dica); // NOVO
    
    p->resposta_correta = atoi(buffer_resposta); // atoi = "ascii to integer"

    fclose(fp);
    return 0; // Sucesso
}


/*
 ====================================================================
  SUA FUNÇÃO 'main' (DO PRIMEIRO CÓDIGO)
  - Agora usando a lógica do TXT
  *** ALTERADO: Implementado o menu completo e as ações especiais ***
 ====================================================================
*/
int main() {
    srand(time(NULL));
    int errou = 0;
    int vitoria = 0;
    int n = 0; // Nível atual (0 a 4)

    // NOVAS: Variáveis para ações especiais (1 = disponível, 0 = usado)
    int acao_pular = 1;
    int acao_trocar = 1;
    int acao_dica = 1;

    // Loop principal do jogo (roda enquanto não errar e não vencer)
    while (errou == 0 && vitoria == 0) {
        
        int pular_esta_pergunta = 0; // Flag para a ação "pular"

    RECARREGAR_PERGUNTA: // Ponto de "goto" para a ação "Trocar"
        if (vitoria || errou) break; // Segurança para não rodar se o jogo acabou

        char menu_choice;
        int numero_pergunta = 0;
        
        // Sorteia a pergunta baseado no nível
        switch (n) {
            case 0: numero_pergunta = (rand() % 5); break;   
            case 1: numero_pergunta = (rand() % 5) + 5; break; 
            case 2: numero_pergunta = (rand() % 5) + 10; break;
            case 3: numero_pergunta = (rand() % 5) + 15; break;
            case 4: numero_pergunta = (rand() % 5) + 20; break;
        }

        Pergunta p; // Cria uma struct para guardar os dados
        if (buscarPerguntaNoTxt(numero_pergunta, &p) != 0) {
            printf("Erro ao ler a pergunta %d. Saindo.\n", numero_pergunta);
            return 1; // Sai do jogo se deu erro
        }
        
    EXIBIR_PERGUNTA: // Ponto de "goto" para "Exibir Pergunta" e "Dica"
        if (vitoria || errou) break; // Segurança

        // Imprime a pergunta lida da struct
        printf("\nNivel %d de %d - Pergunta %d:\n", n + 1, TOTAL_NIVEIS, numero_pergunta + 1);
        printf("%s\n", p.texto); 
        printf("-----------------------------------\n");
        printf("1) %s\n", p.alternativas[0]);
        printf("2) %s\n", p.alternativas[1]);
        printf("3) %s\n", p.alternativas[2]);
        printf("4) %s\n", p.alternativas[3]);
        
        
        // NOVO: Loop do menu de Ação (só roda se a pergunta não foi pulada)
        int pergunta_foi_respondida = 0;
        
        while (pular_esta_pergunta == 0 && pergunta_foi_respondida == 0 && errou == 0 && vitoria == 0) {
        
            printf("\n--- MENU DE ACOES ---\n");
            printf("1) Responder pergunta\n");
            printf("2) Sair do jogo\n");
            printf("3) Utilizar acao especial\n");
            printf("4) Mostrar estado do jogo\n");
            printf("5) Exibir pergunta atual novamente\n");
            printf("Escolha uma opcao: ");
            
            scanf(" %c", &menu_choice);
            
            switch (menu_choice) {
                // [ 1 ] RESPONDER PERGUNTA
                case '1': { 
                    char resposta_usuario_char;
                    printf("Digite sua resposta (1-4): ");
                    scanf(" %c", &resposta_usuario_char);

                    // Converte o char '1' para o int 1
                    int resposta_usuario_int = resposta_usuario_char - '0';

                    // Compara a resposta com a struct
                    if (p.resposta_correta == resposta_usuario_int) {
                        printf("\nCERTA RESPOSTA!\n");
                        n++; // Avança para o próximo nível
                        
                        if (n == TOTAL_NIVEIS) { // Se completou o nível 5
                            printf("\nParabens, VC ganhou!!!!\n");
                            vitoria = 1;
                        }
                    } else {
                        printf("\nResposta errada! A resposta correta era %d. Fim de jogo.\n", p.resposta_correta);
                        errou = 1; // Perde o jogo (só tem 1 vida)
                    }
                    pergunta_foi_respondida = 1; // Sai do loop do menu
                    break;
                }
                
                // [ 2 ] SAIR DO JOGO
                case '2':
                    printf("Saindo do jogo...\n");
                    errou = 1; // Usamos 'errou' para quebrar o loop principal
                    break;
                    
                // [ 3 ] UTILIZAR AÇÃO ESPECIAL
                case '3': {
                    printf("\n--- ACOES ESPECIAIS ---\n");
                    printf("P) Pular Questao (%s)\n", acao_pular ? "Disponivel" : "Usado");
                    printf("T) Trocar Questao (%s)\n", acao_trocar ? "Disponivel" : "Usado");
                    printf("D) Dica (%s)\n", acao_dica ? "Disponivel" : "Usado");
                    printf("V) Voltar ao menu\n");
                    printf("Escolha: ");
                    
                    char acao_choice;
                    scanf(" %c", &acao_choice);
                    
                    if (acao_choice == 'P' || acao_choice == 'p') {
                        if (acao_pular) {
                            printf("\nACAO: Pulando para o proximo nivel...\n");
                            acao_pular = 0; // Gasta a ação
                            n++; // Avança o nível
                            if (n == TOTAL_NIVEIS) { // Se pulou a última pergunta
                                printf("\nParabens, VC ganhou!!!!\n");
                                vitoria = 1;
                            }
                            pular_esta_pergunta = 1; // Sinaliza para o loop externo
                        } else {
                            printf("Acao 'Pular' ja foi utilizada!\n");
                        }
                    } 
                    else if (acao_choice == 'T' || acao_choice == 't') {
                        if (acao_trocar) {
                            printf("\nACAO: Trocando de pergunta...\n");
                            acao_trocar = 0; // Gasta a ação
                            goto RECARREGAR_PERGUNTA; // Volta para sortear outra
                        } else {
                            printf("Acao 'Trocar' ja foi utilizada!\n");
                        }
                    }
                    else if (acao_choice == 'D' || acao_choice == 'd') {
                        if (acao_dica) {
                            printf("\n--- DICA --- \n");
                            printf("%s\n", p.dica); // Imprime a dica lida da struct
                            printf("------------ \n");
                            acao_dica = 0; // Gasta a ação
                            goto EXIBIR_PERGUNTA; // Volta para exibir a pergunta
                        } else {
                            printf("Acao 'Dica' ja foi utilizada!\n");
                        }
                    }
                    else {
                        printf("Voltando ao menu...\n");
                    }
                    break;
                } // Fim case '3'
                
                // [ 4 ] MOSTRAR ESTADO DO JOGO
                case '4':
                    printf("\n--- ESTADO DO JOGO ---\n");
                    printf("Nivel Atual: %d de %d\n", n + 1, TOTAL_NIVEIS);
                    printf("Acoes Especiais Disponiveis:\n");
                    printf(" - Pular: %s\n", acao_pular ? "Sim" : "Nao");
                    printf(" - Trocar: %s\n", acao_trocar ? "Sim" : "Nao");
                    printf(" - Dica: %s\n", acao_dica ? "Sim" : "Nao");
                    printf("-------------------------\n");
                    break;
                    
                // [ 5 ] EXIBIR PERGUNTA ATUAL NOVAMENTE
                case '5':
                    goto EXIBIR_PERGUNTA; // Volta para o label de exibição

                default:
                    printf("Opcao invalida, tente novamente.\n");
                    break;
            }
        } // Fim do loop do menu
    } // Fim do loop do jogo
    
    return 0;
}