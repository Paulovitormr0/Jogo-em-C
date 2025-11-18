#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_LINHA 256
#define TOTAL_NIVEIS 5

typedef struct {
    char texto[MAX_LINHA];
    char alternativas[4][MAX_LINHA];
    char dica[MAX_LINHA]; 
    int resposta_correta; 
} Pergunta;

void ler_string_json(FILE *arquivo, char *texto_destino) {
    char caractere;
    int i = 0;
    
    
    while ((caractere = fgetc(arquivo)) != EOF && caractere != '\"');

    
    while ((caractere = fgetc(arquivo)) != EOF && caractere != '\"') {
        if (i < MAX_LINHA - 1) {
            texto_destino[i++] = caractere;
        }
    }
    texto_destino[i] = '\0'; 
}


int buscar_chave(FILE *arquivo, const char *chave) {
    int tamanho_chave = strlen(chave);
    char caractere;
    int letras_iguais = 0;

   
    while ((caractere = fgetc(arquivo)) != EOF) {
        if (caractere == chave[letras_iguais]) {
            letras_iguais++;
            if (letras_iguais == tamanho_chave) return 1; 
        } else {
           
            if (caractere == chave[0]) {
                letras_iguais = 1; 
            } else {
                letras_iguais = 0;
            }
        }
    }
    return 0; 
}

int buscarPerguntaNoJson(int numero_pergunta, Pergunta *p) {
    FILE *arquivo = fopen("perguntas.json", "r");
    if (arquivo == NULL) {
        printf("ERRO, o arquivo 'perguntas.json' nao foi encontrado na pasta.\n");
        return 1;
    }

    char caractere;
    int contador_objetos = 0;

    
    while ((caractere = fgetc(arquivo)) != EOF) {
        if (caractere == '{') {
            if (contador_objetos == numero_pergunta) {
                break; 
            }
            contador_objetos++;
        }
    }

    if (caractere == EOF) {
        printf("\nERRO: O codigo buscou a pergunta numero %d, mas o arquivo JSON acabou antes.\n", numero_pergunta);
        fclose(arquivo);
        return 1;
    }

    
    if (!buscar_chave(arquivo, "\"texto\"")) return 1;
    ler_string_json(arquivo, p->texto);

    
    if (!buscar_chave(arquivo, "\"alternativas\"")) return 1;
    while ((caractere = fgetc(arquivo)) != EOF && caractere != '['); 
    
    ler_string_json(arquivo, p->alternativas[0]);
    ler_string_json(arquivo, p->alternativas[1]);
    ler_string_json(arquivo, p->alternativas[2]);
    ler_string_json(arquivo, p->alternativas[3]);

    
    if (!buscar_chave(arquivo, "\"resposta\"")) return 1;
    while ((caractere = fgetc(arquivo)) != EOF && caractere != ':'); 
    fscanf(arquivo, "%d", &p->resposta_correta);

    
    if (!buscar_chave(arquivo, "\"dica\"")) return 1;
    ler_string_json(arquivo, p->dica);

    fclose(arquivo);
    return 0;
}

int main() {
    srand(time(NULL));
    int errou = 0;
    int vitoria = 0;
    int nivel_atual = 0; 

    int acao_pular = 1;
    int acao_trocar = 1;
    int acao_dica = 1;

    while (errou == 0 && vitoria == 0) {
        
        int pular_esta_pergunta = 0;

    RECARREGAR_PERGUNTA: 
        if (vitoria || errou) break;

        char opcao_menu; 
        int numero_pergunta = 0;
        
        switch (nivel_atual) {
            case 0: numero_pergunta = (rand() % 5); break;
            case 1: numero_pergunta = (rand() % 5) + 5; break;
            case 2: numero_pergunta = (rand() % 5) + 10; break;
            case 3: numero_pergunta = (rand() % 5) + 15; break;
            case 4: numero_pergunta = (rand() % 5) + 20; break;
        }

        Pergunta p; 
        if (buscarPerguntaNoJson(numero_pergunta, &p) != 0) {
            printf("Erroao ler a pergunta %d. O jogo sera encerrado.\n", numero_pergunta);
            return 1; 
        }
        
    EXIBIR_PERGUNTA: 
        if (vitoria || errou) break; 
        
        printf("\nNivel %d de %d - Pergunta %d (Indice %d):\n", nivel_atual + 1, TOTAL_NIVEIS, (nivel_atual*5)+1, numero_pergunta);
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
            
            scanf(" %c", &opcao_menu);
            
            switch (opcao_menu) {
                
                case '1': { 
                    char resposta_usuario_char;
                    printf("Digite sua resposta (1-4): ");
                    scanf(" %c", &resposta_usuario_char);

                    int resposta_usuario_int = resposta_usuario_char - '0';

                    
                    if (p.resposta_correta == resposta_usuario_int) {
                        printf("\nCERTA RESPOSTA!\n");
                        nivel_atual++; 
                        
                        if (nivel_atual == TOTAL_NIVEIS) { 
                            printf("\nParabens, VC ganhou!!!!\n");
                            vitoria = 1;
                        }
                    } else {
                        printf("\nResposta errada! A resposta correta era %d. Fim de jogo.\n", p.resposta_correta);
                        errou = 1; 
                    }
                    pergunta_foi_respondida = 1; 
                    break;
                }
                
                
                case '2':
                    printf("Saindo do jogo...\n");
                    errou = 1; 
                    break;
                    
                case '3': {
                    printf("\n--- ACOES ESPECIAIS ---\n");
                    printf("P) Pular Questao (%s)\n", acao_pular ? "Disponivel" : "Usado");
                    printf("T) Trocar Questao (%s)\n", acao_trocar ? "Disponivel" : "Usado");
                    printf("D) Dica (%s)\n", acao_dica ? "Disponivel" : "Usado");
                    printf("V) Voltar ao menu\n");
                    printf("Escolha: ");
                    
                    char opcao_acao; 
                    scanf(" %c", &opcao_acao);
                    
                    if (opcao_acao == 'P' || opcao_acao == 'p') {
                        if (acao_pular) {
                            printf("\nACAO: Pulando para o proximo nivel...\n");
                            acao_pular = 0; 
                            nivel_atual++; 
                            if (nivel_atual == TOTAL_NIVEIS) { 
                                printf("\nParabens, VC ganhou!!!!\n");
                                vitoria = 1;
                            }
                            pular_esta_pergunta = 1; 
                        } else {
                            printf("Acao 'Pular' ja foi utilizada!\n");
                        }
                    } 
                    else if (opcao_acao == 'T' || opcao_acao == 't') {
                        if (acao_trocar) {
                            printf("\nACAO: Trocando de pergunta...\n");
                            acao_trocar = 0;
                            goto RECARREGAR_PERGUNTA;
                        } else {
                            printf("Acao 'Trocar' ja foi utilizada!\n");
                        }
                    }
                    else if (opcao_acao == 'D' || opcao_acao == 'd') {
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
                
                case '4':
                    printf("\n--- ESTADO DO JOGO ---\n");
                    printf("Nivel Atual: %d de %d\n", nivel_atual + 1, TOTAL_NIVEIS);
                    printf("Acoes Especiais Disponiveis:\n");
                    printf(" - Pular: %s\n", acao_pular ? "Sim" : "Nao");
                    printf(" - Trocar: %s\n", acao_trocar ? "Sim" : "Nao");
                    printf(" - Dica: %s\n", acao_dica ? "Sim" : "Nao");
                    printf("-------------------------\n");
                    break;
                    
                case '5':
                    goto EXIBIR_PERGUNTA; 

                default:
                    printf("Opcao invalida, tente novamente.\n");
                    break;
            }
        } 
    } 
    return 0;
}