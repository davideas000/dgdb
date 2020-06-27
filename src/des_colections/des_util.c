#include "des_colections/des_util.h"
#include <stdlib.h>
#include <string.h>

char* des_str_dup(const char *str) {
  char *tmp = malloc(strlen(str));
  if (!tmp) {
    return NULL;
  }
  strcpy(tmp, str);
  return tmp;
}
