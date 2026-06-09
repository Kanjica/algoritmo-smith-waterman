#include "fasta.h"

#define TAMANHO_LINHA 1024

char* ler_fasta(const char* caminhoArquivo) {
    FILE* arquivo = fopen(caminhoArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", caminhoArquivo);
        return NULL;
    }

    char linha[TAMANHO_LINHA];
    char* sequencia = NULL;
    size_t tamanho_total = 0;

    while (fgets(linha, sizeof(linha), arquivo)) {
        if (linha[0] == '>') continue; 

        linha[strcspn(linha, "\r\n")] = 0;
        size_t tamanho_linha = strlen(linha);
        if (tamanho_linha == 0) continue;

        char* temp = realloc(sequencia, tamanho_total + tamanho_linha + 1);
        if (temp == NULL) {
            printf("Erro de memória!\n");
            free(sequencia);
            fclose(arquivo);
            return NULL;
        }
        sequencia = temp;
        strcpy(sequencia + tamanho_total, linha);
        tamanho_total += tamanho_linha;
    }

    fclose(arquivo);
    return sequencia;
}