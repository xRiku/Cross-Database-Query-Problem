#include "../include/sort.h"
#include "../include/strings.h"
#include "../include/files.h"
#include "../include/list.h"

/**
 * Comparador para ordenação no quicksort baseado na lista de colunas.
 */
int comparatorFromList(void *a, void *b, void *arg) {
  List *list = arg;
  int *array = getArray(list);
  // Tamanho do vetor arg
  int n = getSize(list);
  // printf("n: %d\n", n);
  char **g1 = *((char ***) a);
  char **g2 = *((char ***) b);
  int result;
  for (int i = 0; i < n; i++) {
    result = strcmp(g1[array[i]], g2[array[i]]);
    if (result != 0) {
      return result;
    }
  }
  return result;
}

/**
 * Cria matriz para validar blocos utilizando quantas linhas cada arquivo tem.
 */
int** validationBlockMatrix(int M, int N, int P, int validationLines) {
  int** pValid = malloc(sizeof(int*) * validationLines);
  for (int i = 0; i < validationLines; i++) {
    pValid[i] = malloc(sizeof(int) * P);
    for (int j = 0; j < P; j++) {
      // Inicializar com 0 cada posição
      pValid[i][j] = 0;
    }
  }
  for (int i = 0; i < validationLines; i++) {
    for (int j = 0; j < P; j++) {
      if (N == 0) {
        continue;
      }
      if (N/M >= 1) {
        pValid[i][j] += M;
        N -= M;
      } else {
        pValid[i][j] = N % M;
        N = 0;
      }
    }
  }
  return pValid;
}


/**
 * Libera o espaço alocado para a matriz de verificação.
 */
void deleteValidationMatrix(int** matrix, int validationLines) {
  for (int i = 0; i < validationLines; i++) {
    free(matrix[i]);
  }
  free(matrix);
}

/**
 * Verifica a menor linha (baseado na lista) da tabela.
 */
// Trocar o nome dessa função
int lowestLine(FILE **pfiles, int P, int M, List* list, int K, int N, int pCopy) {
  int writtenfiles = 0;
  int listLength = getSize(list);
  int *array = getArray(list);
  int nCopy = N;
  int validationLines = (N % (M * P) == 0 ? N / (M * P) : N / (M * P) + 1);
  char ***auxMatrix = createMemoMatrix(P, K);
  int **pValid = validationBlockMatrix(M, N, P, validationLines);

  // for (int i = 0; i < validationLines; i++) {
  //   for (int j = 0; j < P; j++) {
  //     printf("%d ", pValid[i][j]);
  //   }
  //   putchar('\n');
  // }

  char *line = NULL;
  long unsigned int n = 0;
  int firstStringIndex = -1;
  
  char **firstString = malloc(sizeof(char*) * K);
  for (int i = 0; i < K; i ++) {
    firstString[i] = malloc(sizeof(char) * SLOTS);
  }

  for (int a = 0; a < validationLines; a++) {
    // printf("Linha: %d\n", a);
    //leitura 1
    for (int i = 0; i < P; i++) {
      // printf("%d %d\n", a, i);
      if (pValid[a][i] == 0) {
        continue;
      }   
      getline(&line, &n, pfiles[P - pCopy + i]);
      // printf("Aqui %s", line);
      if (feof(pfiles[P - pCopy + i])) {
        break;
      }
      char *token = strtok(line, ",");
      for (int j = 0; j < K; j ++) {
        if (token[strlen(token) - 1] == '\n') {
          char *lineAux = malloc(sizeof(char) * SLOTS);
          for (unsigned k = 0; k < strlen(token); k++) {
            if (token[k] == '\n') {
              lineAux[k] = '\0';  
              break;
            }
            lineAux[k] = token[k];
          }
          strcpy(auxMatrix[i][j], lineAux);
          free(lineAux);
        } else {
          strcpy(auxMatrix[i][j], token);
        }
        token = strtok(NULL, ",");
      }
      
    }

    int halt = 0;
    for (int i = 0; halt != P; i++) {
      if (i == 0) {
        halt = 0;
      }
      if (i < P && pValid[a][i] == 0){
        halt++;
        continue;
      }
      if (i < P && pValid[a][i] != 0 && i == firstStringIndex) {
        getline(&line, &n, pfiles[P - pCopy + i]);
        char *token = strtok(line, ",");
        for (int j = 0; j < K; j ++) {
          if (token[strlen(token) - 1] == '\n') {
            char *lineAux = malloc(sizeof(char) * SLOTS);
            for (unsigned k = 0; k < strlen(token); k++) {
              if (token[k] == '\n') {
                lineAux[k] = '\0';  
                break;
              }
              lineAux[k] = token[k];
            }
            strcpy(auxMatrix[i][j], lineAux);
            free(lineAux);
          } else {
            strcpy(auxMatrix[i][j], token);
          }
          token = strtok(NULL, ",");
        }
        firstStringIndex = -1;
      }
      
      
      if (i == P ) {
        int valid = -1;
        for (int j = 0; j < P; j++) {
          if (pValid[a][j] != 0) {
            valid = j;
            break;
          }
        }
        for (int j = 0; j < K; j++) {
          strcpy(firstString[j], auxMatrix[valid][j]);
        }
        firstStringIndex = valid;
        for (int b = 0; b < P; b++) {
          if (pValid[a][b] == 0) {
            continue;
          }
          for (int j = 0; j < K; j++) {
            int result = 0;
            for (int k = 0; k < listLength; k++) {
                result = strcmp(firstString[array[k]], auxMatrix[b][array[k]]);
              if (result != 0) {
                break;
              }
            }
            if (result > 0) {
              firstStringIndex = b;
              for (int k = 0; k < K; k++) {
                strcpy(firstString[k], auxMatrix[b][k]);
              }
            }
          }
        }

        pValid[a][firstStringIndex]--;
        int chosenFile = (a + 1) % P == 0 ? P - 1 : ((a + 1) % P ) - 1;
        for (int w = 0; w < K; w++) {
          if (w == K - 1) {
            fprintf(pfiles[chosenFile + pCopy], "%s\n", firstString[w]);
          } else {
            fprintf(pfiles[chosenFile + pCopy], "%s,", firstString[w]);
          }
        }
        i = -1;
      }
      
    }
    firstStringIndex = -1;
  }
  
  // Libera o que foi alocado.
  for (int i = 0; i < K; i ++) {
    free(firstString[i]);
  }
  free(firstString);

  free(line);
  deleteValidationMatrix(pValid, validationLines);
  deleteMemoMatrix(auxMatrix, P, K);
  return validationLines;
}

/**
 * Compara os blocos para ordenação e finaliza quando só sobra um arquivo escrito.
 */
void compareBlock(FILE** pfiles, int P, int M, List* list, int K, int N, int order) {
  int writtenFiles = 0;
  int i = 0;
  int pCopy = 0;
  while (writtenFiles != 1) {
    if (pCopy == 0) {
      writtenFiles = lowestLine(pfiles, P, pow(P, i) * M, list, K, N, pCopy);
      pCopy = P;
    } else {
      writtenFiles = lowestLine(pfiles, P, pow(P, i) * M, list, K, N, pCopy);
      pCopy = 0;
    }
    rewindFiles(pfiles, 2*P);
    i++;
  }

  renameFile(pCopy, P, order);
}

/**
 * Realiza a ordenação externa intercalada.
 * Foi escolhido o uso de qsort_r para poder passar a lista de colunas como argumento.
 */
void externalSorting(FILE *file, int M, int P, List *list, int order) {
  int N = 0;
  int listLength = getSize(list); 
  // Número de iterações da memoria ate o arquivo terminar.
  FILE **pfiles = createFiles(2 * P);
  char *fileName = malloc(sizeof(char)*16);
  for (int i = 0; i < 2 * P; i++) {
    sprintf(fileName, "f%d.txt", i);
    openFile(&pfiles[i], fileName, "w+");
  }
  int K = wordsPerLine(file);
  char ***matrix = createMemoMatrix(M, K);
  char *line = NULL;
  long unsigned int n = 0;
  int pCopy = P;
  int preventRepeated = 0;
  int halt;
  while(!feof(file)) {
    // Saber em que iteração parar para imprimir no arquivo.
    halt = -1;
    for (int i = 0; i < M; i++) {
      getline(&line, &n, file);
      if (feof(file)) {
          halt = i;
        break;
      }
      char *token = strtok(line, ",");
      for (int j = 0; j < K; j++) {
        // Para excluir o '\n' do fim da linha.
        if (token[strlen(token) - 1] == '\n') {
          char *lineAux = malloc(sizeof(char) * SLOTS);
          for (unsigned k = 0; k < strlen(token); k++) {
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
      // Número de páginas
      N++;
    }

    // Se halt for diferente de -1 é preciso ler as linhas remanecentes do tamanho de halt.
    if (halt != -1) {
      char ***auxMatrix = createMemoMatrix(halt, K);
      for (int i = 0; i < halt; i++) {
        for (int j = 0; j < K; j++) {
          strcpy(auxMatrix[i][j], matrix[i][j]);
        }
      }
      char ***auxMatrix2 = matrix;
      matrix = auxMatrix;
      auxMatrix = NULL;
      deleteMemoMatrix(auxMatrix2, M, K);
    }

    if (halt == - 1) {
      // qsort padrão (M linhas).
      qsort_r(matrix, M, sizeof(matrix[0]), comparatorFromList, list);
    } else {
      // Ultimo qsort com halt linhas.
      qsort_r(matrix, halt, sizeof(matrix[0]), comparatorFromList, list);
    }
    for (int i = 0; i < M; i++) {
      if (i == halt) {
        // Para de escrever no arquivo para não pegar de uma área out of bounds.
        // Visto que halt <= M;
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
  for (int i = 0; i < P; i++) {
    rewind(pfiles[P+i]);
  }

  // Realiza a comparação iterativa dos blocos.
  compareBlock(pfiles, P, M, list, K, N, order);

  // Realiza a liberação do que foi alocado.
  free(line);
  deleteMemoMatrix(matrix, halt == -1 ? M : halt, K);
  closeFiles(pfiles, 2 * P);
  free(fileName);
}

