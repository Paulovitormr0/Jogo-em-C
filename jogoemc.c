#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define LINHAS_POR_PERGUNTA 8 
#define MAX_LINHA 256
#define TOTAL_NIVEIS 5

typedef struct {
    char texto[MAX_LINHA];
    char alternativas[4][MAX_LINHA];
    char dica[MAX_LINHA]; 
    int resposta_correta; 
} Pergunta;

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

    int linhas_para_pular = numero_pergunta * LINHAS_POR_PERGUNTA;
    char buffer_lixo[MAX_LINHA];
    
    for (int i = 0; i < linhas_para_pular; i++) {
        if (fgets(buffer_lixo, MAX_LINHA, fp) == NULL) {
            printf("ERRO: Arquivo terminou antes de achar a pergunta %d\n", numero_pergunta);
            fclose(fp);
            return 1;
        }
    }


    if (fgets(p->texto, MAX_LINHA, fp) == NULL) return 1;           
    if (fgets(p->alternativas[0], MAX_LINHA, fp) == NULL) return 1; 
    if (fgets(p->alternativas[1], MAX_LINHA, fp) == NULL) return 1; 
    if (fgets(p->alternativas[2], MAX_LINHA, fp) == NULL) return 1; 
    if (fgets(p->alternativas[3], MAX_LINHA, fp) == NULL) return 1; 
    

    char buffer_resposta[MAX_LINHA];
    if (fgets(buffer_resposta, MAX_LINHA, fp) == NULL) return 1; 


    if (fgets(p->dica, MAX_LINHA, fp) == NULL) return 1; 
   

    trim_newline(p->texto);
    trim_newline(p->alternativas[0]);
    trim_newline(p->alternativas[1]);
    trim_newline(p->alternativas[2]);
    trim_newline(p->alternativas[3]);
    trim_newline(p->dica); 
    
    p->resposta_correta = atoi(buffer_resposta); // atoi = "ascii to integer"

    fclose(fp);
    return 0; 
}

int main() {
    srand(time(NULL));
    int errou = 0;
    int vitoria = 0;
    int n = 0; 

   
    int acao_pular = 1;
    int acao_trocar = 1;
    int acao_dica = 1;

    while (errou == 0 && vitoria == 0) {
        
        int pular_esta_pergunta = 0;

    RECARREGAR_PERGUNTA: 
        if (vitoria || errou) break;

        char menu_choice;
        int numero_pergunta = 0;
        
        switch (n) {
            case 0: numero_pergunta = (rand() % 5); break;   
            case 1: numero_pergunta = (rand() % 5) + 5; break; 
            case 2: numero_pergunta = (rand() % 5) + 10; break;
            case 3: numero_pergunta = (rand() % 5) + 15; break;
            case 4: numero_pergunta = (rand() % 5) + 20; break;
        }

        Pergunta p; 
        if (buscarPerguntaNoTxt(numero_pergunta, &p) != 0) {
            printf("Erro ao ler a pergunta %d. Saindo.\n", numero_pergunta);
            return 1; 
        }
        
    EXIBIR_PERGUNTA: 
        if (vitoria || errou) break; 

        
        printf("\nNivel %d de %d - Pergunta %d:\n", n + 1, TOTAL_NIVEIS, numero_pergunta + 1);
        printf("%s\n", p.texto); 
        printf("-----------------------------------\n");
        printf("1) %s\n", p.alternativas[0]);
        printf("2) %s\n", p.alternativas[1]);
        printf("3) %s\n", p.alternativas[2]);
        printf("4) %s\n", p.alternativas[3]);
        
        
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

                    int resposta_usuario_int = resposta_usuario_char - '0';

                    
                    if (p.resposta_correta == resposta_usuario_int) {
                        printf("\nCERTA RESPOSTA!\n");
                        n++; 
                        
                        if (n == TOTAL_NIVEIS) { 
                            printf("\nParabens, VC ganhou!!!!\n");
                            vitoria = 1;
                        }
                    } else {
                        printf("\nResposta errada! A resposta correta era %d. Fim de jogo.\n", p.resposta_correta);
                        errou = 1; 
                    }
                    pergunta_foi_respondida = 1; // Sai do loop do menu
                    break;
                }
                
                // [ 2 ] SAIR DO JOGO
                case '2':
                    printf("Saindo do jogo...\n");
                    errou = 1; 
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
                            acao_pular = 0; 
                            n++; 
                            if (n == TOTAL_NIVEIS) { 
                                printf("\nParabens, VC ganhou!!!!\n");
                                vitoria = 1;
                            }
                            pular_esta_pergunta = 1; 
                        } else {
                            printf("Acao 'Pular' ja foi utilizada!\n");
                        }
                    } 
                    else if (acao_choice == 'T' || acao_choice == 't') {
                        if (acao_trocar) {
                            printf("\nACAO: Trocando de pergunta...\n");
                            acao_trocar = 0;
                            goto RECARREGAR_PERGUNTA;
                        } else {
                            printf("Acao 'Trocar' ja foi utilizada!\n");
                        }
                    }
                    else if (acao_choice == 'D' || acao_choice == 'd') {
                        if (acao_dica) {
                            printf("\n--- DICA --- \n");
                            printf("%s\n", p.dica); 
                            printf("------------ \n");
                            acao_dica = 0; 
                            goto EXIBIR_PERGUNTA; 
                        } else {
                            printf("Acao 'Dica' ja foi utilizada!\n");
                        }
                    }
                    else {
                        printf("Voltando ao menu...\n");
                    }
                    break;
                }
                
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
                    goto EXIBIR_PERGUNTA; 

                default:
                    printf("Opcao invalida, tente novamente.\n");
                    break;
            }
        } // Fim do loop do menu
    } // Fim do loop do jogo
    
    return 0;
}