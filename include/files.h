#ifndef _FILE_
#define _FILE_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "list.h"

FILE **createFiles(int n);

void openFile(FILE **file, char *name, char* type);

void rewindFiles(FILE **files, int);

void renameFile(int fileValue, int P, int order);

void mergeFiles(char* outputFile, List *List1, List *List2);

void closeFiles(FILE **, int);

int wordsPerLine(FILE *file);

void deletePFiles(int P);

#endif