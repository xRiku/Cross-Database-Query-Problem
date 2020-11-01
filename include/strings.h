#ifndef _STRINGS_
#define _STRINGS_

#include <stdlib.h>
#include <string.h>

#define SLOTS 51

void deleteMemoMatrix(char ***matrix, int M, int K);

char ***createMemoMatrix(int M, int K);

int countCommas(char *);


#endif