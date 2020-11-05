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
 * Responsável pela renomeação dos arquivos ordenados.
 */
void renameFile(int fileValue, int order) {
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
      strcpy(oldname ,"f3.txt");
      strcpy(newName ,"ordered0.txt");
    } else {
      strcpy(oldname ,"f3.txt");
      strcpy(newName ,"ordered1.txt");
    }
  }
  rename(oldname, newName);
  free(oldname);
  free(newName);
  //     //checar se existe com o mesmo nome
  //     char oldname[] = "f3.txt";
  //     char newname[] = "ordenado2.txt";
  //     rename(oldname, newname);
  //   } else {
  //     char oldname[] = "f3.txt";
  //     char newname[] = "ordenado1.txt";
  //     rename(oldname, newname);
  //   }
  // } else {
  //     char oldname[] = "f0.txt";
  //     char newname[] = "ordenado2.txt";
  //     rename(oldname, newname);
  //   } else {
  //     char oldname[] = "f0.txt";
  //     char newname[] = "ordenado1.txt";
  //     rename(oldname, newname);
  //   }
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


