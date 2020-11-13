#include "../include/strings.h"


/**
 * Aloca espaço para a matriz de caracteres.
 */
char ***createMemoMatrix(int M, int K) {
  char ***matrix = malloc(sizeof(char**) * M);
  for (int i = 0; i < M; i++) {
    matrix[i] = malloc(sizeof(char*) * K);
    for (int j = 0; j < K; j++) {
      // Valor máximo arbitrário para as strings
      matrix[i][j] = malloc(sizeof(char) * SLOTS);
    }
  }
  return matrix;
}

/**
 * Remove o espaço alocado para a matriz na memória.
 */
void deleteMemoMatrix(char ***matrix, int M, int K) {
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < K; j++) {
      free(matrix[i][j]);
    }
    free(matrix[i]);
  }
  free(matrix);
}

/**
 * Conta o número de vírgulas na string.
 */
int countCommas(char *str) {
  int n = 0;
  for (int i = 0; str[i] != '\0'; i++) {
    if (str[i] == ',') {
      n += 1;
    }
  }
  return n;
}

/**
 * Escreve a linha na matriz
 */
void writeMatrixLine(int K, char **matrix, char* line) {
  char *token = strtok(line, ",");
  for (int j = 0; j < K; j++) {
    // Para excluir o '\n' do fim da linha.
    unsigned tokenLength = strlen(token);
    if (tokenLength + 1 > SLOTS) {
      matrix[j] = (char *) realloc(matrix[j], tokenLength + 1);
    }
    if (token[tokenLength - 1] == '\n') {
      char *lineAux = malloc(sizeof(char) * (tokenLength + 1));
      for (unsigned k = 0; k < tokenLength; k++) {
        if (token[k] == '\n') {
          lineAux[k] = '\0';  
          break;
        }
        lineAux[k] = token[k];
      }
      strcpy(matrix[j], lineAux);
      free(lineAux);
    } else {
      strcpy(matrix[j], token);
    }
    token = strtok(NULL, ",");
  }
}
