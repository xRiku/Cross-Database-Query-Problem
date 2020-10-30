#include "../include/sort.h"
#include "../include/strings.h"

/**
 * Converte a lista em um vetor de inteiros.
 */
int *listToIntArray(char *L, int length) {
  int *intList = malloc(sizeof(char *) * length);
  char *token = strtok(L, ",");
  int i = 0;
  while (token != NULL) {
    intList[i] = atoi(token);
    i++;
    token = strtok(NULL, ",");
  }
  return intList;
}

/**
 * Libera o espaço alocado para o vetor.
 */
void deleteIntArray(int *L) {
  free(L);
}

/**
 * Calcula o número de palavras por linha para ser alocado em memória
 */
int wordsPerLine(FILE *file) {
  // Evitar erro no valgrind
  char *line = NULL;
  long unsigned int n = 0;
  getline(&line, &n, file);
  int K = countCommas(line) + 1;
  // printf("Line %s, length: %d\n", line, strlen(line));
  rewind(file);
  free(line);
  return K;
}

/**
 * Aloca espaço para a matriz de caracteres
 */
char ***createMemoMatrix(int M, int K) {
  char ***matrix = malloc(sizeof(char**) * M);
  for (int i = 0; i < M; i++) {
    matrix[i] = malloc(sizeof(char*) * K);
  }
  return matrix;
}

/**
 * Realiza a ordenação externa intercalada
 */
void externalSorting(FILE *file, int M, int P) {
  int K = wordsPerLine(file);
  printf("K = %d\n", K);
  char ***matrix = createMemoMatrix(M, K);

}

