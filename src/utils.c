#include "utils.h"
#include <stdlib.h>
#include <string.h>

char *r_strcat(char *s1, char *s2) {
  char *sf = malloc(strlen(s1) + strlen(s2) + 1);
  strcpy(sf, s1);
  strcat(sf, s2);

  return sf;
}

size_t strnlen(const char *str, size_t maxlen) {
  for (size_t i = 0; i < maxlen; i++) {
    if (str[i] == '\0') return i;
  }

  return maxlen;
}

char *ns_strncpy(const char *str, size_t maxlen) {
  size_t len = strnlen(str, maxlen);
  char *ns = malloc(len + 1);
  strncpy(ns, str, len);
  ns[len] = '\0';
  return ns;
}
