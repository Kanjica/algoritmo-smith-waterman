#include "smithWaterman.h"

#define ANSI_COR_VERMELHO "\x1b[31m"
#define ANSI_COR_AZUL     "\x1b[34m"
#define ANSI_COR_RESET    "\x1b[0m"

ConfigPontuacao definirPontuacoesPadrao() {
    ConfigPontuacao p = { 
        .match = 1, 
        .mismatch = -1, 
        .gap = -1 
    };
    return p;
}

void smithWaterman(DadosAlinhamento* dados, ConfigPontuacao p, int imprimirMatriz) {
    Celula **matriz;
    MaxScore max_score = initMaxScore();
    int aux;

    matriz = initMatriz(dados, p);

    // printf("entrei aq ó");

    //PARTE 1 - PREENCHIMENTO DA MATRIZ
    for(int i = 1; i < dados->linhas; i++){
        for(int j = 1; j < dados->colunas; j++) {
            // printf("i: %d, j: %d\n", i, j);
            int score_diagonal = matriz[i-1][j-1].score + ((dados->seq1[i-1] == dados->seq2[j-1]) ? p.match : p.mismatch);
            // printf("score");
            int score_up = matriz[i-1][j].score + p.gap;
            // printf("score2");
            int score_left = matriz[i][j-1].score + p.gap;
            // printf("score3");

            // matriz[i][j].score = max(score_diagonal, score_up, score_left);

            aux = max(score_diagonal, score_up, score_left);

            if(aux == 0) {
                matriz[i][j].score = 0;
                matriz[i][j].direcao = NONE;
            } else if(aux == score_diagonal) {
                matriz[i][j].score = score_diagonal;
                matriz[i][j].direcao = DIAGONAL;
            } else if(aux == score_up) {
                matriz[i][j].score = score_up;
                matriz[i][j].direcao = CIMA;
            } else {
                matriz[i][j].score = score_left;
                matriz[i][j].direcao = ESQUERDA;
            }

            // PARTE 2 - RASTREAMENTO DO MELHOR SCORE
            attMaxScore(&max_score, matriz[i][j].score, i, j);
        }
    }
    
    if(imprimirMatriz){
        printMatriz(matriz, dados);
    }

    // PARTE 3 - TRACEBACK
    int tam_max = dados->linhas + dados->colunas;
    char alin_seq1[tam_max];
    char alin_seq2[tam_max];
    int index = 0; 

    int i = max_score.i;
    int j = max_score.j;

    while (matriz[i][j].direcao != NONE) {
        
        if (matriz[i][j].direcao == DIAGONAL) {
            alin_seq1[index] = dados->seq1[i - 1];
            alin_seq2[index] = dados->seq2[j - 1];
            i--; 
            j--;
        } 
        else if (matriz[i][j].direcao == CIMA) {
            alin_seq1[index] = dados->seq1[i - 1];
            alin_seq2[index] = '-';
            i--;
        } 
        else if (matriz[i][j].direcao == ESQUERDA) {
            alin_seq1[index] = '-';
            alin_seq2[index] = dados->seq2[j - 1];
            j--;
        }
        
        index++;
    }
    
    alin_seq1[index] = '\0';
    alin_seq2[index] = '\0';

    printResultadoAlinhamento(alin_seq1, alin_seq2, index, max_score);

    liberarMatriz(matriz, dados->linhas);
}

Celula** initMatriz(DadosAlinhamento* dados, ConfigPontuacao p){
    Celula **matriz;

    matriz = (Celula**) malloc(dados->linhas * sizeof(Celula*));
    
    for(int i = 0; i < dados->linhas; i++) {
        matriz[i] = (Celula*) calloc(dados->colunas, sizeof(Celula));
    }
    
    // print_matriz(matriz, dados);
    // printf("\nLinhas (n): %d\nColunas (m): %d\n\n\n\n\n", dados->linhas, dados->colunas);

    return matriz;
    // liberar_matriz(matriz, dados->linhas);
}

void printMatriz(Celula** matriz, DadosAlinhamento* dados) {
    printf("        "); 
    for(int j = 0; j < strlen(dados->seq2); j++) {
        printf("%3c ", dados->seq2[j]);
    }
    printf("\n");

    printf("    ");
    for(int j = 0; j < dados->colunas; j++) {
        int valor = matriz[0][j].score;
        
        if (valor == 0) {
            printf("%s%3d%s ", ANSI_COR_VERMELHO, valor, ANSI_COR_RESET);
        } else {
            printf("%s%3d%s ", ANSI_COR_AZUL, valor, ANSI_COR_RESET);
        }
    }
    printf("\n");

    for(int i = 1; i < dados->linhas; i++) {
        printf("%3c ", dados->seq1[i - 1]); 
        
        for(int j = 0; j < dados->colunas; j++) {
            int valor = matriz[i][j].score;
            
            if (valor == 0) {
                printf("%s%3d%s ", ANSI_COR_VERMELHO, valor, ANSI_COR_RESET);
            } else {
                printf("%s%3d%s ", ANSI_COR_AZUL, valor, ANSI_COR_RESET);
            }
        }
        printf("\n");
    }
    printf("\n\n");
}

void liberarMatriz(Celula** matriz, int num_linhas) {
    for(int i = 0; i < num_linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

int max(int a, int b, int c) {
    int max_val = (a > b)? a : b;
    max_val = (max_val > c)? max_val : c;
    return (max_val > 0)? max_val : 0; 
}

MaxScore initMaxScore() {
    MaxScore max_score;
    max_score.value = 0;
    max_score.i = 0;
    max_score.j = 0;
    return max_score;
}

void attMaxScore(MaxScore* max_score, int score, int i, int j) {
    if(score > max_score->value) {
        max_score->value = score;
        max_score->i = i;
        max_score->j = j;
    }
}

ConfigPontuacao definirPontuacoes(int match, int mismatch, int gap) {
    ConfigPontuacao p = { 
        .match = match, 
        .mismatch = mismatch, 
        .gap = gap 
    };
    return p;
}

void printResultadoAlinhamento(char* alin_seq1, char* alin_seq2, int index, MaxScore max_score) {
    printf("\n=================== RESULTADO DO ALINHAMENTO LOCAL ===================\n\n");
    
    // 1. Printa a Seq 1 (de trás para frente)
    printf("Seq 1: ");
    for (int k = index - 1; k >= 0; k--) {
        printf("%c", alin_seq1[k]);
    }
    printf("\n");

    // 2. Printa a Linha de Conexão (Matches/Mismatches)
    int matches = 0;
    printf("       "); // Alinhamento com o "Seq 1: "
    for (int k = index - 1; k >= 0; k--) {
        if (alin_seq1[k] == alin_seq2[k] && alin_seq1[k] != '-') {
            printf("|");
            matches++;
        } else {
            printf(" "); // Mismatch ou Gap
        }
    }
    printf("\n");

    // 3. Printa a Seq 2 (de trás para frente)
    printf("Seq 2: ");
    for (int k = index - 1; k >= 0; k--) {
        printf("%c", alin_seq2[k]);
    }
    printf("\n\n");

    // 4. Relatório de Estatísticas
    float identidade = 0.0;
    if (index > 0) {
        identidade = ((float)matches / index) * 100.0;
    }
    
    printf("----------------------------------------------------------------------\n");
    printf("Score Máximo do Alinhamento : %d\n", max_score.value);
    printf("Comprimento do Alinhamento  : %d resíduos/bases\n", index);
    printf("Identidade Local            : %d/%d (%.2f%%)\n", matches, index, identidade);
    printf("Posição de Início na Matriz : [%d][%d]\n", max_score.i, max_score.j);
    printf("======================================================================\n");
}