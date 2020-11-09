#include "../include/files.h"
#include "../include/strings.h"
#include "../include/sort.h"
#include "../include/list.h"



int main(int argc, char** argv) {
  if (argc != 8) {
    return 1;
  }
  int P = atoi(argv[1]);
  int M = atoi(argv[2]);
  int listLength = countCommas(argv[3]) + 1;

  //Listas para query nos bancos
  List* L1 = createList(argv[3], listLength);
  List* L2 = createList(argv[4], listLength);
  FILE **files = createFiles(2);
  openFile(&files[0], argv[5], "r");
  openFile(&files[1], argv[6], "r");
  externalSorting(files[0], M, P, L1, 0);
  externalSorting(files[1], M, P, L2, 1);
  mergeFiles(argv[7], L1, L2);
  
  // deletePFiles();
  deleteList(L1);
  deleteList(L2);
  closeFiles(files, 2);
  // for (int i = 0; i < 2 * P; i++) {
  //   sprintf(fileName, "f%d.txt", i);
  //   remove(fileName);
  // }
  return 0;
}