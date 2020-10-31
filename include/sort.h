#ifndef _SORT_
#define _SORT_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *listToIntArray(char *L, int length);

void deleteIntArray(int *L);

int wordsPerLine(FILE *file);

void externalSorting(FILE *file, int M, int P, int *list, int listLength);

void deleteMemoMatrix(char ***matrix, int M, int K);


#endif