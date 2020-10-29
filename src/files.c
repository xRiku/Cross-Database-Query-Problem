#include "../include/files.h"

FILE **openFiles(char **argv) {
  FILE **files = malloc(sizeof(FILE*) * 3);
  files[0] = fopen(argv[5], "r");
  files[1] = fopen(argv[6], "r");
  files[2] = fopen(argv[7], "w");
  return files;
}


void closeFiles(FILE **files) {
  for (int i = 0; i < 3; i++) {
    fclose(files[i]);
  }
}