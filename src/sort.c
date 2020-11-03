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
 * Calcula o número de palavras por linha para ser alocado em memória.
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
  // Tamanho do vetor arg
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
 * Verifica a menor linha (baseado na lista) da tabela
 */
int lowestLine(FILE **pfiles, int P, int M, int* list, int K, int N) {
  int listLength = sizeof(list)/sizeof(list[0]);
  int nCopy = N;
  int validationLines = (N%(M*P) == 0 ? N/(M*P) : N/(M*P) + 1);
  char ***auxMatrix = createMemoMatrix(P, K);
  int **pValid = validationBlockMatrix(M, N, P, validationLines);

  /* for (int i = 0; i < validationLines; i++) {
    for (int j = 0; j < P; j++) {
      printf("%d ", pValid[i][j]);
    }
    putchar('\n');
  } */

  char *line = NULL;
  long unsigned int n = 0;
  /* for (int i = 0; i < P; i++) {
    getline(&line, &n, pfiles[P + i]);
    if (feof(pfiles[P + i])) {
      break;
    }
    
    char *token = strtok(line, ",");
    for (int j = 0; j < K; j ++) {
      if (token[strlen(token) - 1] == '\n') {
        char *lineAux = malloc(sizeof(char) * SLOTS);
        for (int k = 0; k < strlen(token); k++) {
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
  } */

  


  int firstStringIndex = -1;
  
  char **firstString = malloc(sizeof(char*) * K);
  for (int i = 0; i < K; i ++) {
    firstString[i] = malloc(sizeof(char) * SLOTS);
  }

  for (int i = 0; i < K; i++) {
    strcpy(firstString[i], auxMatrix[0][i]);
  }
  

  //Função de tirar o menor funcional
  /* for (int i = 0; i < P; i++) {
    if (pValid[a][i] == 0){
      continue;
    } 
    // if (i == P - 2) {
    //   break;
    // }
    for (int j = 0; j < K; j++) {
      int result = 0;
      for (int k = 0; k < listLength; k++) {
        int intg1 = atoi(firstString[list[k]]);
        int intg2 = atoi(auxMatrix[i][list[k]]);
        if (intg1 != 0) {
          result = intg1 - intg2;
        } else {
          result = strcmp(firstString[j][k], (auxMatrix[i][j][k]));
        }
        if (result != 0) {
          break;
        }
      }
      if (result > 0) {
        firstStringIndex = i;
        for (int k = 0; k < K; k++) {
          strcpy(firstString[k], auxMatrix[i][k]);
        }
      }
    }
  } */

  for (int a = 0; a < validationLines; a++) {
    //leitura 1
    for (int i = 0; i < P; i++) {
      if (pValid[a][i] == 0) {
        continue;
      }
      getline(&line, &n, pfiles[P + i]);
      if (feof(pfiles[P + i])) {
        break;
      }
      char *token = strtok(line, ",");
      for (int j = 0; j < K; j ++) {
        if (token[strlen(token) - 1] == '\n') {
          char *lineAux = malloc(sizeof(char) * SLOTS);
          for (int k = 0; k < strlen(token); k++) {
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

    for (int i = 0; i < P; i++) {
      for (int j = 0; j < K; j++) {
        printf("%s ", auxMatrix[i][j]);
      }
    putchar('\n');
    }
    char **firstString = malloc(sizeof(char*) * K);
    for (int i = 0; i < K; i ++) {
      firstString[i] = malloc(sizeof(char) * SLOTS);
    }

    int halt = 0;
    for (int i = 0; halt != P; i++) {
      printf("Entrou aqui: %d\n", i);
      if (i == 0) {
        halt = 0;
      }
      // halt++;
      if (i < P && pValid[a][i] == 0){
        halt++;
        continue;
      }
      
      if (pValid[a][i] != 0 && i == firstStringIndex) {
        printf("Entrou, a: %d i: %d\n", a, i);
        getline(&line, &n, pfiles[P + i]);
        char *token = strtok(line, ",");
        for (int j = 0; j < K; j ++) {
          if (token[strlen(token) - 1] == '\n') {
            char *lineAux = malloc(sizeof(char) * SLOTS);
            for (int k = 0; k < strlen(token); k++) {
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
      // if (i == P - 2) {
      //   break;
      // }
      
      
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
              int intg1 = atoi(firstString[list[k]]);
              int intg2 = atoi(auxMatrix[b][list[k]]);
              if (intg1 != 0) {
                result = intg1 - intg2;
              } else {
                result = strcmp(firstString[j][k], (auxMatrix[b][j][k]));
              }
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
        printf("Melhor %d\n", firstStringIndex);
        for (int w = 0; w < K; w++) {
          if (w == K - 1) {
            fprintf(pfiles[a], "%s\n", firstString[w]);
          } else {
            fprintf(pfiles[a], "%s,", firstString[w]);
          }
        }
        i = -1;
      }
      
    }
    firstStringIndex = -1;
  }
  for (int i = 0; i < P; i++) {
    for (int j = 0; j < K; j++) {
      printf("%s ", auxMatrix[i][j]);
    }
    putchar('\n');
  }
  // Contador de blocos

  // printf("Indice vencedor: %d\n", firstStringIndex);
  // for (int i = 0; i < K; i++) {
  //   printf("%s ", firstString[i]);
  // }
  // putchar('\n');

  free(line);
}

/**
 * Compara os blocos (M linhas de cada arquivo) para ordenação
 */
void compareBlock(FILE** pfiles, int P, int M, int* list, int K, int N) {
  printf("Maior: %d\n", lowestLine(pfiles, P, M, list, K, N));
}

/**
 * Realiza a ordenação externa intercalada.
 */
void externalSorting(FILE *file, int M, int P, int *list, int listLength) {
  int N = 0;
  // Número de iterações da memoria ate o arquivo terminar
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
    //Saber em que iteração parar para imprimir no arquivo
    int halt = -1;
    for (int i = 0; i < M; i++) {
      getline(&line, &n, file);
      if (feof(file)) {
        halt = i;
        break;
      }
      char *token = strtok(line, ",");
      for (int j = 0; j < K; j++) {
        // Para excluir o '\n' do fim da linha
        if (token[strlen(token) - 1] == '\n') {
          char *lineAux = malloc(sizeof(char) * SLOTS);
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
      // Número de páginas
      N++;
    }
    qsort_r(matrix, M, sizeof(matrix[0]), comparatorFromList, list);
    for (int i = 0; i < M; i++) {
      if (i == halt) {
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
  // Até aqui da pra por em uma função talvez
  
  compareBlock(pfiles, P, M, list, K, N);

  // Print da matrix
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < K; j++) {
      printf("%s ", matrix[i][j]);
    }
    putchar('\n');
  }

  free(line);
  free(fileName);
  deleteMemoMatrix(matrix, M, K);
  closeFiles(pfiles, 2*P);

}

