#include "../include/files.h"


/**
 * Realiza a abertura dos arquivos
 */
FILE **openFiles(char *file1, char *file2, char* file3) {
  FILE **files = malloc(sizeof(FILE*) * 3);
  files[0] = fopen(file1, "r");
  if(files[0] == NULL){
		fprintf(stderr, "Erro ao abrir %s\n", file1);
		exit(1);
	}
  files[1] = fopen(file2, "r");
  if(files[1] == NULL){
		fprintf(stderr, "Erro ao abrir %s\n", file2);
		exit(1);
	}
  files[2] = fopen(file3, "w");
  if(files[2] == NULL){
		fprintf(stderr, "Erro ao abrir %s\n", file3);
		exit(1);
	}
  return files;
}

/**
 * Realiza o fechamento dos arquivos.
 */
void closeFiles(FILE **files) {
  for (int i = 0; i < 3; i++) {
    fclose(files[i]);
  }
  free(files);
}

/**
 * Conta o número de vírgulas na string
 */
int countCommas(char *List) {
  int n = 0;
  for (int i = 0; List[i] != '\0'; i++) {
    if (List[i] == ',') {
      n += 1;
    }
  }
  return n;
}

/**
 * Converte a lista em um vetor de inteiros.
 */
int *listToIntArray(char *L, int length) {
  int *intList = malloc(sizeof(int) * length);
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

