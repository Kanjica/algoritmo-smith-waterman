#ifndef SMITH_WATERMAN_H
#define SMITH_WATERMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int match;
    int mismatch;
    int gap;
} ConfigPontuacao;

typedef struct {
    char* seq1;
    char* seq2;
    int linhas;   // strlen(seq1) + 1
    int colunas;  // strlen(seq2) + 1
} DadosAlinhamento;

typedef struct {
    int value;
    int i;
    int j;
} MaxScore;

enum Direcao {
    NONE,
    DIAGONAL,
    CIMA,
    ESQUERDA,
};

typedef struct {
    int score;
    enum Direcao direcao;
} Celula;

ConfigPontuacao definirPontuacoesPadrao();
ConfigPontuacao definirPontuacoes(int match, int mismatch, int gap);
Celula** initMatriz(DadosAlinhamento* dados, ConfigPontuacao p);
void printMatriz(Celula** matriz, DadosAlinhamento* dados);
void liberarMatriz(Celula** matriz, int num_linhas);
int max(int a, int b, int c);
void smithWaterman(DadosAlinhamento* dados, ConfigPontuacao p, int imprimir_matriz);
MaxScore initMaxScore();
void attMaxScore(MaxScore* max_score, int score, int i, int j);
void printResultadoAlinhamento(char* alin_seq1, char* alin_seq2, int index, MaxScore max_score);
#endif // SMITH_WATERMAN_H