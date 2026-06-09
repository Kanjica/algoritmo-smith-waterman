#include "smithWaterman.h"
#include "fasta.h"

int main(int argc, char *argv[]) {
    char* s1 = ler_fasta("orf6_yp394.fasta");
    char* s2 = ler_fasta("orf6_bhy586.fasta");

    // char* s1 = ler_fasta("homo_sapiens_NM_000558_5.fasta");
    // char* s2 = ler_fasta("pan_troglodytes_X00226_1.fasta");

    if (!s1 || !s2){printf("erropainho");return 1;}

    DadosAlinhamento dados = {
        .seq1 = s1,
        .seq2 = s2,
        .linhas = strlen(s1) + 1,
        .colunas = strlen(s2) + 1
    };

    ConfigPontuacao p;

    if (argc == 4){
        // Ex: ./app 2 -1 -3 (match, mismatch, gap)
        int match = atoi(argv[1]);
        int mismatch = atoi(argv[2]);
        int gap = atoi(argv[3]);
        p = definirPontuacoes(match, mismatch, gap);
        
    } 
    else if (argc == 2){
        // Ex: ./app -2 (passou apenas o gap, assume match=1 e mismatch=-1)
        int gap = atoi(argv[1]);
        p = definirPontuacoes(1, -1, gap);
        
    } 
    else if (argc == 1){
        p = definirPontuacoesPadrao();
    } 
    else{
        printf("parametros inválidos. Uso: ./app [match] [mismatch] [gap]\n");
        free(s1);
        free(s2);
        return 1;
    }

    int imprimirMatriz = 1; // 1 para imprimir, 0 para não imprimir

    printf("imprimir matriz? (1 para sim, 0 para não): ");
    scanf("%d", &imprimirMatriz);

    smithWaterman(&dados, p, imprimirMatriz); // O último parâmetro é para imprimir a matriz

    free(s1);
    free(s2);
    return 0;
}