#include "../include/files.h"


int main(int argc, char** argv) {
  // if (argc != 8) {
  //   return 1;
  // }
  int P = atoi(argv[1]);
  int M = atoi(argv[2]);
  FILE **files = openFiles(argv);
  closeFiles(files);
  return 0;
}