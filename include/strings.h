#ifndef _STRINGS_
#define _STRINGS_

#include <stdlib.h>
#include <string.h>

#define SLOTS 51

char ***createMemoMatrix(int M, int K);

void deleteMemoMatrix(char ***matrix, int M, int K);

int countCommas(char *);


#endif