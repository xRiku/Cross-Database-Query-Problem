#include "../include/strings.h"


/**
 * Aloca espaço para a matriz de caracteres
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
 * Conta o número de vírgulas na string
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
