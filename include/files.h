#ifndef _FILE_
#define _FILE_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

FILE **createFiles(int n);

void openFile(FILE **file, char *name, char* type);

void rewindFiles(FILE **files, int);

void closeFiles(FILE **, int);



#endif