#include "../include/strings.h"




/**
 * Conta o número de vírgulas na string
 */
int countCommas(char *str) {
  int n = 0;
  for (int i = 0; str[i] != '\0'; i++) {
    if (str[i] == ',') {
      n += 1;
    }
  }
  return n;
}
