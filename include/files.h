#ifndef _FILE_
#define _FILE_

#include <stdlib.h>
#include <stdio.h>

FILE **openFiles(char *file1, char *file2, char* file3);

void closeFiles(FILE **);

int countCommas(char *);

#endif