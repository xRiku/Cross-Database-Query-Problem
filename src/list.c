#include "../include/list.h"


/** Necessário para ter a informação do tamanho do vetor dinamico
 *  na hora de passar como argumento no qsort_r.
 */ 

struct list {
  int size;
  int *array;
};


/**
 * Inicia uma lista com o vetor.
 */
List *createList(char *L, int length) {
  List *list = malloc(sizeof(List));
  int n = 0;
  list->array = strToIntArray(L, length, &n);
  list->size = n;
  return list;
}

/**
 * Converte a lista em um vetor de inteiros,
 * modifica o ponteiro size para ter o tamanho do vetor.
 */
int *strToIntArray(char *L, int length, int *size) {
  int *intList = malloc(sizeof(char *) * length);
  char *token = strtok(L, ",");
  int i = 0;
  while (token != NULL) {
    intList[i] = atoi(token);
    i++;
    token = strtok(NULL, ",");
  }
  *size = i;
  return intList;
}

/**
 * Retorna o vetor da struct.
 */
int* getArray(List *list) {
  return list->array;
}

/**
 * Retorna o tamanho do vetor.
 */
int getSize(List *list) {
  return list->size;
}

/**
 * Libera o espaço alocado pela struct.
 */
void deleteList(List *list) {
  deleteIntArray(list->array);
  free(list);
}


/**
 * Libera o espaço alocado para o vetor.
 */
void deleteIntArray(int *array) {
  free(array);
}

