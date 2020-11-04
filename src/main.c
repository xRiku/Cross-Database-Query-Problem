#include "../include/files.h"
#include "../include/strings.h"
#include "../include/sort.h"



int main(int argc, char** argv) {
  if (argc != 8) {
    return 1;
  }
  int P = atoi(argv[1]);
  int M = atoi(argv[2]);
  int listLength = countCommas(argv[3]) + 1;

  //Listas para query nos bancos
  int *L1 = listToIntArray(argv[3], listLength);
  int *L2 = listToIntArray(argv[4], listLength);

  FILE **files = createFiles(3);
  openFile(&files[0], argv[5], "r");
  openFile(&files[1], argv[6], "r");
  openFile(&files[2], argv[7], "w");
  externalSorting(files[0], M, P, L1, listLength);

  deleteIntArray(L1);
  deleteIntArray(L2);
  closeFiles(files, 3);
  return 0;
}