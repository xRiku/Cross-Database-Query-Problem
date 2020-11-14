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
int blockSorting(FILE **pfiles, int P, int M, List* list, int K, int N, int pCopy) {
  int listLength = getSize(list);
  int *array = getArray(list);
  int validationLines = (N % (M * P) == 0 ? N / (M * P) : N / (M * P) + 1);
  char ***auxMatrix = createMemoMatrix(P, K);
  int **pValid = validationBlockMatrix(M, N, P, validationLines);

  char *line = NULL;
  long unsigned int n = 0;
  int firstStringIndex = -1;
  
  char **firstString = malloc(sizeof(char*) * K);
  for (int i = 0; i < K; i ++) {
    firstString[i] = malloc(sizeof(char) * SLOTS);
  }

  // O entendimento dessa função é um pouco complicado visto que foi feito de forma iterativa,
  // portanto possui muitos laços de repetição.
  // As comparações são feitas baseado na matriz de validação, quando o elemento da posição é 0 não se pode fazer mais leituras.
  // É uma matriz que possui como linhas o número de iterações necessárias para ler todas as linhas restantes
  // e como colunas P dispositivos, onde cada elemento representa o número de linhas a serem lidas daquele bloco.
  for (int a = 0; a < validationLines; a++) {
    //leitura 1
    for (int i = 0; i < P; i++) {
      if (pValid[a][i] == 0) {
        continue;
      }   
      getline(&line, &n, pfiles[P - pCopy + i]);
      if (feof(pfiles[P - pCopy + i])) {
        break;
      }
      writeMatrixLine(K, auxMatrix[i], line);
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
        if (feof(pfiles[P - pCopy + i])) {
          break;
        }
        writeMatrixLine(K, auxMatrix[i], line);
        firstStringIndex = -1;
      }
      
      // Verifica se a posição é valida para pegar a primeira string.
      if (i == P ) {
        int valid = -1;
        for (int j = 0; j < P; j++) {
          if (pValid[a][j] != 0) {
            valid = j;
            break;
          }
        }
        for (int j = 0; j < K; j++) {
          unsigned length = strlen(auxMatrix[valid][j]);
          // Realoca se ultrapassar SLOTS
          if (length + 1 > SLOTS) {
            firstString[j] = realloc(firstString[j], length + 1);
          }
          strcpy(firstString[j], auxMatrix[valid][j]);
        }
        firstStringIndex = valid;
        
        // Decide qual string entre as linhas do bloco é a menor.
        for (int b = 0; b < P; b++) {
          // Se for 0 quer dizquer que não poder ler mais linhas.
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
                unsigned length = strlen(auxMatrix[b][k]);
                if (length + 1 > SLOTS) {
                  firstString[k] = realloc(firstString[k], length + 1);
                }
                strcpy(firstString[k], auxMatrix[b][k]);
              }
            }
          }
        }
        
        // Diminui uma leitura disponivel da menor string.
        // Escreve a atual no arquivo.
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
void compareBlocks(FILE** pfiles, int P, int M, List* list, int K, int N, int order) {
  int writtenFiles = 0;
  int i = 0;
  int pCopy = 0;
  // Interrompe o laço quando o número de arquivos escritos for 1,
  // ou seja, o arquivo que sobrou é o ordenado.
  while (writtenFiles != 1) {
    if (pCopy == 0) {
      writtenFiles = blockSorting(pfiles, P, pow(P, i) * M, list, K, N, pCopy);
      pCopy = P;
    } else {
      writtenFiles = blockSorting(pfiles, P, pow(P, i) * M, list, K, N, pCopy);
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
  // Número de páginas
  int N = 0;
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
  int halt = -1;
  while(!feof(file)) {
    // Saber em que iteração parar para imprimir no arquivo.
    halt = -1;
    for (int i = 0; i < M; i++) {
      getline(&line, &n, file);
      if (feof(file)) {
        halt = i;
        break;
      }
      writeMatrixLine(K, matrix[i], line);
      N++;
    }

    // Se halt for diferente de -1 é preciso ler as linhas remanecentes do tamanho de halt.
    if (halt != -1) {
      char ***auxMatrix = createMemoMatrix(halt, K);
      for (int i = 0; i < halt; i++) {
        for (int j = 0; j < K; j++) {
          unsigned length = strlen(matrix[i][j]);
          if (length + 1 > SLOTS) {
            auxMatrix[i][j] = (char *) realloc(auxMatrix[i][j], length + 1);
          }
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
        // Visto que halt <= M.
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
  compareBlocks(pfiles, P, M, list, K, N, order);

  // Realiza a liberação do que foi alocado.
  free(line);
  deleteMemoMatrix(matrix, halt == -1 ? M : halt, K);
  closeFiles(pfiles, 2 * P);
  free(fileName);
}

