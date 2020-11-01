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
 * Comparador para ordenação no quicksort baseado na lista de colunas.
 */
int comparatorFromList(void *a, void *b, void *arg) {
  int *list = arg;
  int n = sizeof(list)/sizeof(int);
  char **g1 = *((char ***) a);
  char **g2 = *((char ***) b);
  int result;
  for (int i = 0; i < n; i++) {
    int intg1 = atoi(g1[list[i]]);
    int intg2 = atoi(g2[list[i]]);
    if (intg1 != 0) {
      result = intg1 - intg2; 
    } else {
      result = strcmp(g1[i], g2[i]);
    }
    if (result != 0) {
      return result;
    }
  }
  return result;
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
    openFiles(&pfiles[i], fileName, "w+");
  }
  int K = wordsPerLine(file);
  // printf("K = %d\n", K);
  char ***matrix = createMemoMatrix(M, K);
  char *line = NULL;
  long unsigned int n = 0;
  int pCopy = P;
  while(!feof(file)) {
    int halve = -1;
    for (int i = 0; i < M; i++) {
      getline(&line, &n, file);
      if (feof(file)) {
        halve = i;
        break;
      }
      char *token = strtok(line, ",");
      for (int j = 0; j < K; j++) {
        if (token[strlen(token) - 1] == '\n') {
          char *lineAux = malloc(sizeof(char)*31);
          for (int k = 0; k < strlen(token); k++) {
            if (token[k] == '\n') {
              lineAux[k] = '\0';  
              break;
            }
            lineAux[k] = token[k];
          }
          strcpy(matrix[i][j], lineAux);
          free(lineAux);
        } else {
          strcpy(matrix[i][j], token);
        }
          token = strtok(NULL, ",");
      }
    }
    qsort_r(matrix, M, sizeof(matrix[0]), comparatorFromList, list);
    for (int i = 0; i < M; i++) {
      if (i == halve) {
        break;
      }
      for (int j = 0; j < K; j++) {
        if (j == K - 1) {
          fprintf(pfiles[pCopy], "%s\n", matrix[i][j]);
        } else {
          fprintf(pfiles[pCopy], "%s,", matrix[i][j]);
        }
      }
    }
    if (pCopy == 2*P - 1) {
      pCopy = P;
    } else {
      pCopy++;
    }
  }
  free(line);
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < K; j++) {
      printf("%s ", matrix[i][j]);
    }
    putchar('\n');
  }

  free(fileName);
  deleteMemoMatrix(matrix, M, K);
  closeFiles(pfiles, 2*P);

}

