#ifndef _SORT_
#define _SORT_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

int *listToIntArray(char *L, int length);

void deleteIntArray(int *L);

int wordsPerLine(FILE *file);

void compareBlock(FILE** pfiles, int P, int M, int* list, int K, int N, int order);

int** validationBlockMatrix(int M, int N, int P, int validationLines);

void deleteValidationMatrix(int** matrix, int validationLines);

int lowestLine(FILE **pfiles, int P, int M, int* list, int K, int N, int pCopy);

void externalSorting(FILE *file, int M, int P, int *list, int listLength, int order);

#endif