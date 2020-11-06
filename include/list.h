#ifndef _LIST_
#define _LIST_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct list List;

List *createList(char *L, int length);

int* getArray(List *list);

int getSize(List *list);

int *strToIntArray(char *L, int length, int *size);

void deleteList(List *list);

void deleteIntArray(int *array);



#endif