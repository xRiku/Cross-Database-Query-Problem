#include "../include/files.h"


/**
 * Realiza a alocação para um vetor de Arquivos
 */
FILE **createFiles(int n) {
  FILE **files = malloc(sizeof(FILE*) * n);
  return files;
}


/**
 * Realiza a abertura dos arquivos
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
 * Realiza o fechamento dos arquivos.
 */
void closeFiles(FILE **files, int n) {
  for (int i = 0; i < n; i++) {
    fclose(files[i]);
  }
  free(files);
}


