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

  // for (int i = 0; i < listLength; i++) {
  //   printf("%d ", L1[i]);
  // }
  // putchar('\n');
  // for (int i = 0; i < listLength; i++) {
  //   printf("%d ", L2[i]);
  // }
  // putchar('\n');

  FILE **files = createFiles(3);
  openFiles(&files[0], argv[5], "r");
  openFiles(&files[1], argv[6], "r");
  openFiles(&files[2], argv[7], "w");
  externalSorting(files[0], M, P);

  deleteIntArray(L1);
  deleteIntArray(L2);
  closeFiles(files, 3);
  return 0;
}