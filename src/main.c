#include "../include/files.h"


int main(int argc, char** argv) {
  // if (argc != 8) {
  //   return 1;
  // }
  // int P = atoi(argv[1]);
  // int M = atoi(argv[2]);
  int listLength = countCommas(argv[1]) + 1;
  int *L1 = listToIntArray(argv[1], listLength);
  int *L2 = listToIntArray(argv[2], listLength);

  for (int i = 0; i < listLength; i++) {
    printf("%d ", L1[i]);
  }
  putchar('\n');
  for (int i = 0; i < listLength; i++) {
    printf("%d ", L2[i]);
  }
  putchar('\n');

  FILE **files = openFiles(argv[3], argv[4], argv[5]);
  char *line;
  long unsigned int n = 0;
  getline(&line, &n, files[0]);
  // printf("%s\n", line);
  closeFiles(files);
  return 0;
}