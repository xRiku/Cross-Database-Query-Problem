#include "../include/files.h"

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
 * Conta o número de vírgulas na string L1 
 */
int countCommas(char *L1) {
  int n = 0;
  for (int i = 0; L1[i] != '\0'; i++) {
    if (L1[i] == ',') {
      n += 1;
    }
  }
  return n;
}

void closeFiles(FILE **files) {
  for (int i = 0; i < 3; i++) {
    fclose(files[i]);
  }
}