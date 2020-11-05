#include "../include/files.h"
#include "../include/strings.h"

/**
 * Realiza a alocação para um vetor de Arquivos
 */
FILE **createFiles(int n) {
  FILE **files = malloc(sizeof(FILE*) * n);
  return files;
}


/**
 * Realiza a abertura de um arquivo
 */
void openFile(FILE **file, char *name, char* type) {
  *file = fopen(name, type);
  if(*file == NULL){
		fprintf(stderr, "Erro ao abrir %s\n", name);
		exit(1);
	}
}

/**
 * Realiza rewind em todos os arquivos
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
  char *oldname = malloc(sizeof(char)*16);
  char *newName = malloc(sizeof(char)*13);
  if (fileValue) {
    if (order == 0) {
      strcpy(oldname ,"f0.txt");
      strcpy(newName ,"ordered0.txt");
    } else {
      strcpy(oldname ,"f0.txt");
      strcpy(newName ,"ordered1.txt");
    }
  } else {
    if (order == 0) {
      sprintf(oldname, "f%d.txt", P);
      // strcpy(oldname ,"f3.txt");
      strcpy(newName ,"ordered0.txt");
    } else {
      sprintf(oldname, "f%d.txt", P);
      strcpy(newName ,"ordered1.txt");
    }
  }
  rename(oldname, newName);
  free(oldname);
  free(newName);
}

/**
 * Une os arquivos no arquivo de saída baseado em suas querys
 */
void mergeFiles(char* outputFile, int *L1, int *L2, int listLength) {
  FILE *outFile = fopen(outputFile, "w");
  if(outFile == NULL){
		fprintf(stderr, "Erro ao abrir %s\n", outputFile);
		exit(1);
	}
  FILE *input1 = fopen("ordered0.txt", "r");
  if(input1 == NULL){
		fprintf(stderr, "Erro ao abrir %s\n", "ordered0");
		exit(1);
	}
  FILE *input2 = fopen("ordered1.txt", "r");
  if(input2 == NULL){
		fprintf(stderr, "Erro ao abrir %s\n", "ordered1");
		exit(1);
	}
  int K1 = wordsPerLine(input1);
  printf("K1: %d\n", K1);
  int K2 = wordsPerLine(input2);
  printf("K1: %d\n", K2);
  char *line = NULL;
  long unsigned int n = 0;
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
      strcpy(matrix2[j], lineAux);
      free(lineAux);
    } else {
      strcpy(matrix2[j], token);
    }
      token = strtok(NULL, ",");
  }
  for (int i = 0; i < K1; i ++) {
    printf("%s ", matrix1[i]);
  }
  putchar('\n');
  for (int i = 0; i < K2; i ++) {
    printf("%s ", matrix2[i]);
  }
  putchar('\n');



  while (1) {
    int result = 0;
    for (int i = 0; i < listLength; i++) {
      int intg1 = atoi(matrix1[L1[i]]);
      int intg2 = atoi(matrix2[L2[i]]);
      if (intg1 != 0 && intg2 != 0) {
        result = intg1 - intg2;
      } else {
        result = strcmp(matrix1[L1[i]], matrix2[L2[i]]);
      }
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
          strcpy(matrix1[j], lineAux);
          free(lineAux);
        } else {
          strcpy(matrix1[j], token);
        }
        token = strtok(NULL, ",");
      }
    }
    if (result == 0) {
      int j = 0;
      for (int i = 0; i < listLength; i++) {
        // if (i == listLength - 1) {
          if (j == 0) {
            fprintf(outFile, "%s", matrix1[L1[i]]);
            j++;
          } else {
            fprintf(outFile, ",%s", matrix1[L1[i]]);
          }
          strcpy(matrix1[L1[i]], "-");
          strcpy(matrix2[L2[i]], "-");
        
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
          strcpy(matrix2[j], lineAux);
          free(lineAux);
        } else {
          strcpy(matrix2[j], token);
        }
          token = strtok(NULL, ",");
      }

    }
  }
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


