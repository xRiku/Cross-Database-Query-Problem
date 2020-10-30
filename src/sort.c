#include "../include/sort.h"
#include "../include/strings.h"
#include "../include/files.h"

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
 * Realiza a ordenação externa intercalada
 */
void externalSorting(FILE *file, int M, int P) {
  FILE **pfiles = createFiles(2 * P);
  char *fileName = malloc(sizeof(char)*16);
  for (int i = 0; i < 2 * P; i++) {
    sprintf(fileName, "f%d.txt", i);
    openFiles(&pfiles[i], fileName, "w");
  }
  int K = wordsPerLine(file);
  // printf("K = %d\n", K);
  char ***matrix = createMemoMatrix(M, K);
  char *line = NULL;
  long unsigned int n = 0;
  for (int i = 0; i < M; i++) {
    getline(&line, &n, file);
    char *token = strtok(line, ",");
    for (int j = 0; j < K; j++) {
      strcpy(matrix[i][j],token);
      token = strtok(NULL, ",");
    }
  }
  // putchar('\n');
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < K; j++) {
      printf("%s ", matrix[i][j]);
    }
  }

  free(fileName);
  closeFiles(pfiles, 2*P);

}

