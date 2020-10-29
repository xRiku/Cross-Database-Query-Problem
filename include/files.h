#ifndef _FILE_
#define _FILE_

#include <stdlib.h>
#include <stdio.h>

FILE **openFiles(char **argv);

void closeFiles(FILE **);

#endif