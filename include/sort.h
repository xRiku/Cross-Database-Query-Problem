#ifndef _SORT_
#define _SORT_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "list.h"




int wordsPerLine(FILE *file);

void compareBlock(FILE** pfiles, int P, int M, List* list, int K, int N, int order);

int** validationBlockMatrix(int M, int N, int P, int validationLines);

void deleteValidationMatrix(int** matrix, int validationLines);

int lowestLine(FILE **pfiles, int P, int M, List* list, int K, int N, int pCopy);

void externalSorting(FILE *file, int M, int P, List* list, int order);

#endif