#ifndef _SORT_
#define _SORT_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "list.h"

void compareBlocks(FILE** pfiles, int P, int M, List* list, int K, int N, int order);

int** validationBlockMatrix(int M, int N, int P, int validationLines);

void deleteValidationMatrix(int** matrix, int validationLines);

int blockSorting(FILE **pfiles, int P, int M, List* list, int K, int N, int pCopy);

void externalSorting(FILE *file, int M, int P, List* list, int order);

#endif