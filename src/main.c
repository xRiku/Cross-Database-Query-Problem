#include "../include/files.h"


int main(int argc, char** argv) {
  // if (argc != 8) {
  //   return 1;
  // }
  // int P = atoi(argv[1]);
  // int M = atoi(argv[2]);
  int commas = countCommas(argv[1]);
  printf("%d\n", commas);
  FILE **files = openFiles(argv[2], argv[3], argv[4]);
  char *line;
  long unsigned int n = 0;
  getline(&line, &n, files[0]);
  printf("%s\n", line);
  closeFiles(files);
  return 0;
}