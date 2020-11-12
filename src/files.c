#include "../include/files.h"
#include "../include/strings.h"

/**
 * Realiza a alocação para um vetor de arquivos.
 */
FILE **createFiles(int n) {
  FILE **files = malloc(sizeof(FILE*) * n);
  return files;
}


/**
 * Realiza a abertura de um arquivo.
 */
void openFile(FILE **file, char *name, char* type) {
  *file = fopen(name, type);
  if(*file == NULL){
		fprintf(stderr, "Erro ao abrir %s\n", name);
		exit(1);
	}
}

/**
 * Realiza rewind em todos os arquivos.
 */
void rewindFiles(FILE **files, int n) {
  for (int i = 0; i < n; i ++) {
    rewind(files[i]);
  }
}

/**
 * Responsável pela renomeação dos arquivos ordenados.
 */
void renameFile(int fileValue, int P, int order) {
  // Tamanho 16 para cobrir o tamanho máximo de um inteiro.
  char *oldname = malloc(sizeof(char)*16);
  // Tamanho 13 fixo dos arquivos de saida.
  char *newName = malloc(sizeof(char)*13);
  if (fileValue) {
    if (order == 0) {
      strcpy(oldname ,"f0.txt");
      strcpy(newName ,"sorted0.txt");
    } else {
      strcpy(oldname ,"f0.txt");
      strcpy(newName ,"sorted1.txt");
    }
  } else {
    if (order == 0) {
      sprintf(oldname, "f%d.txt", P);
      strcpy(newName ,"sorted0.txt");
    } else {
      sprintf(oldname, "f%d.txt", P);
      strcpy(newName ,"sorted1.txt");
    }
  }
  rename(oldname, newName);
  free(oldname);
  free(newName);
}

/**
 * Une os arquivos no arquivo de saída baseado nas posições de cada lista.
 */
void mergeFiles(char* outputFile, List *L1, List *L2) {
  int *array1 = getArray(L1);
  int *array2 = getArray(L2);
  int listLength = getSize(L1);
  FILE *outFile = fopen(outputFile, "w");
  if(outFile == NULL){
		fprintf(stderr, "Erro ao abrir %s\n", outputFile);
		exit(1);
	}
  FILE *input1 = fopen("sorted0.txt", "r");
  if(input1 == NULL){
		fprintf(stderr, "Erro ao abrir %s\n", "sorted0");
		exit(1);
	}
  FILE *input2 = fopen("sorted1.txt", "r");
  if(input2 == NULL){
		fprintf(stderr, "Erro ao abrir %s\n", "sorted1");
		exit(1);
	}
  int K1 = wordsPerLine(input1);
  int K2 = wordsPerLine(input2);
  char *line = NULL;
  long unsigned int n = 0;
  // Guarda apenas uma linha de cada arquivo.
  char **matrix1 = malloc(sizeof(char*) * K1);
  char **matrix2 = malloc(sizeof(char*) * K2);
  for (int i = 0; i < K1; i ++) {
    matrix1[i] = malloc(sizeof(char) * SLOTS);    
  }
  for (int i = 0; i < K2; i ++) {
    matrix2[i] = malloc(sizeof(char) * SLOTS);
  }
  getline(&line, &n, input1);
  char *token = strtok(line, ",");
  for (int j = 0; j < K1; j++) {
    // Para excluir o '\n' do fim da linha.
    unsigned tokenLength = strlen(token);
    if (token[tokenLength - 1] == '\n') {
      char *lineAux = malloc(sizeof(char) * (tokenLength + 1));
      for (unsigned k = 0; k < tokenLength; k++) {
        if (token[k] == '\n') {
          lineAux[k] = '\0';  
          break;
        }
        lineAux[k] = token[k];
      }
      strcpy(matrix1[j], lineAux);
      free(lineAux);
    } else {
      strcpy(matrix1[j], token);
    }
    token = strtok(NULL, ",");
  }
  getline(&line, &n, input2);
  token = strtok(line, ",");
  for (int j = 0; j < K2; j++) {
    // Para excluir o '\n' do fim da linha.
    unsigned tokenLength = strlen(token);
    if (token[tokenLength - 1] == '\n') {
      char *lineAux = malloc(sizeof(char) * (tokenLength + 1));
      for (unsigned k = 0; k < tokenLength; k++) {
        if (token[k] == '\n') {
          lineAux[k] = '\0';  
          break;
        }
        lineAux[k] = token[k];
      }
      strcpy(matrix2[j], lineAux);
      free(lineAux);
    } else {
      strcpy(matrix2[j], token);
    }
      token = strtok(NULL, ",");
  }
  // Aqui acontece o algortimo de merge proposto no pdf.
  while (1) {
    int result = 0;
    // O resultado de result indica em qual arquivo acontecerá o getline.
    // Se derem match a leitura da proxima linha será feita em ambas.
    for (int i = 0; i < listLength; i++) {
        result = strcmp(matrix1[array1[i]], matrix2[array2[i]]);
      if (result != 0) {
        break;
      }
    }
    if (result > 0) {
      getline(&line, &n, input2);
      if (feof(input2)) {
        break;
      }
      token = strtok(line, ",");
      for (int j = 0; j < K2; j++) {
        // Para excluir o '\n' do fim da linha.
        unsigned tokenLength = strlen(token);
        if (token[tokenLength - 1] == '\n') {
          char *lineAux = malloc(sizeof(char) * (tokenLength + 1));
          for (unsigned k = 0; k < tokenLength; k++) {
            if (token[k] == '\n') {
              lineAux[k] = '\0';  
              break;
            }
            lineAux[k] = token[k];
          }
          strcpy(matrix2[j], lineAux);
          free(lineAux);
        } else {
          strcpy(matrix2[j], token);
        }
          token = strtok(NULL, ",");
      }
    }
    if (result < 0) {
      getline(&line, &n, input1);
      if (feof(input1)) {
        break;
      }
      char *token = strtok(line, ",");
      for (int j = 0; j < K1; j++) {
        // Para excluir o '\n' do fim da linha.
        unsigned tokenLength = strlen(token);
        if (token[tokenLength - 1] == '\n') {
          char *lineAux = malloc(sizeof(char) * (tokenLength + 1));
          for (unsigned k = 0; k < tokenLength; k++) {
            if (token[k] == '\n') {
              lineAux[k] = '\0';  
              break;
            }
            lineAux[k] = token[k];
          }
          strcpy(matrix1[j], lineAux);
          free(lineAux);
        } else {
          strcpy(matrix1[j], token);
        }
        token = strtok(NULL, ",");
      }
    }
    // Se as listas derem match, a posição correspondente da lista em cada linha
    // é lida, o elemento da posição escrito no arquivo de saída e a posição marcada com "-". 
    // Depois é feito uma leitura para verificar se já foi marcado. Se não foi, o elemento da posição é escrito no arquivo.
    if (result == 0) {
      int j = 0;
      for (int i = 0; i < listLength; i++) {
          if (j == 0) {
            fprintf(outFile, "%s", matrix1[array1[i]]);
            j++;
          } else {
            fprintf(outFile, ",%s", matrix1[array1[i]]);
          }
          strcpy(matrix1[array1[i]], "-");
          strcpy(matrix2[array2[i]], "-");
      }
      for (int i = 0; i < K1; i++) {
        if (strcmp(matrix1[i], "-") != 0) {
          if (j == 0) {
            fprintf(outFile, "%s", matrix1[i]);
            j++;
          } else {
            fprintf(outFile, ",%s", matrix1[i]);
          }
        }
      }
      for (int i = 0; i < K2; i++) {
        if (strcmp(matrix2[i], "-") != 0) {
          if (j == 0) {
            fprintf(outFile, "%s", matrix2[i]);
            j++;
          } else {
            fprintf(outFile, ",%s", matrix2[i]);
          }
        }
      }
      fprintf(outFile, "\n");
      getline(&line, &n, input1);

      if (feof(input1)) {
        break;
      }
      char *token = strtok(line, ",");
      for (int j = 0; j < K1; j++) {
        // Para excluir o '\n' do fim da linha.
        unsigned tokenLength = strlen(token);
        if (token[tokenLength - 1] == '\n') {
          char *lineAux = malloc(sizeof(char) * (tokenLength + 1));
          for (unsigned int k = 0; k < tokenLength; k++) {
            if (token[k] == '\n') {
              lineAux[k] = '\0';  
              break;
            }
            lineAux[k] = token[k];
          }
          strcpy(matrix1[j], lineAux);
          free(lineAux);
        } else {
          strcpy(matrix1[j], token);
        }
        token = strtok(NULL, ",");
      }
      getline(&line, &n, input2);
      if (feof(input2)) {
        break;
      }
      token = strtok(line, ",");
      for (int j = 0; j < K2; j++) {
        // Para excluir o '\n' do fim da linha.
        unsigned tokenLength = strlen(token);
        if (token[tokenLength - 1] == '\n') {
          char *lineAux = malloc(sizeof(char) * (tokenLength + 1));
          for (unsigned k = 0; k < tokenLength; k++) {
            if (token[k] == '\n') {
              lineAux[k] = '\0';  
              break;
            }
            lineAux[k] = token[k];
          }
          strcpy(matrix2[j], lineAux);
          free(lineAux);
        } else {
          strcpy(matrix2[j], token);
        }
        token = strtok(NULL, ",");
      }
    }
  }
  // Liberando o que foi alocado.
  free(line);
  for (int i = 0; i < K1; i ++) {
    free(matrix1[i]);    
  }
  for (int i = 0; i < K2; i ++) {
    free(matrix2[i]);    
  }
  free(matrix1);
  free(matrix2);
  fclose(outFile);
  fclose(input1);
  fclose(input2);
}


/**
 * Realiza o fechamento dos arquivos.
 */
void closeFiles(FILE **files, int n) {
  for (int i = 0; i < n; i++) {
    fclose(files[i]);
  }
  free(files);
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
 * Deleta os 2P arquivos remanescentes.
 */
void deletePFiles(int P) {
  char *fileName = malloc(sizeof(char)*16);
  for (int i = 0; i < 2 * P; i++) {
    sprintf(fileName, "f%d.txt", i);
    if (access(fileName, F_OK) != -1) {
      remove(fileName);
    }
  }
  free(fileName);
}

