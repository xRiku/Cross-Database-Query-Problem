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
  rewind(file);
  free(line);
  return K;
}

/**
 * Comparador para ordenação no quicksort
 */
int comparatorFromList(void *a, void *b) {
  char **g1 = *((char ***) a);
  char **g2 = *((char ***) b);
  int intg1 = atoi(g1[0]);
  int intg2 = atoi(g2[0]);
  if (intg1 != 0) {
    return intg2 - intg1; 
  }
  return strcmp(g2[0], g1[0]);
}

/**
 * Remove o espaço alocado para a matriz na memória
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
 * Realiza a ordenação externa intercalada
 */
void externalSorting(FILE *file, int M, int P, int *list, int listLength) {
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
    if (feof(file)) {
      break;
    }
    char *token = strtok(line, ",");
    for (int j = 0; j < K; j++) {
      strcpy(matrix[i][j],token);
      token = strtok(NULL, ",");
    }
  }
  free(line);
  qsort(matrix, M, sizeof(matrix[0]), comparatorFromList);
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < K; j++) {
      printf("%s ", matrix[i][j]);
    }
  }

  free(fileName);
  deleteMemoMatrix(matrix, M, K);
  closeFiles(pfiles, 2*P);

}

