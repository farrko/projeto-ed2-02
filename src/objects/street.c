#include <stdlib.h>
#include <string.h>

#include "objects/street.h"

#define CEP_LEN 16
#define NOME_LEN 64

struct street_t {
  char ldir[CEP_LEN];
  int has_ldir;
  char lesq[CEP_LEN];
  int has_lesq;
  double cmp;
  double vm;
  char nome[NOME_LEN];
};

static int is_absent(const char *cep) {
  return cep == NULL || (cep[0] == '-' && cep[1] == '\0');
}

street_t *street_init(const char *ldir, const char *lesq, double cmp, double vm, const char *nome) {
  street_t *street = calloc(1, sizeof(street_t));

  street->has_ldir = !is_absent(ldir);
  if (street->has_ldir) strncpy(street->ldir, ldir, CEP_LEN - 1);

  street->has_lesq = !is_absent(lesq);
  if (street->has_lesq) strncpy(street->lesq, lesq, CEP_LEN - 1);

  street->cmp = cmp;
  street->vm = vm;
  strncpy(street->nome, nome, NOME_LEN - 1);

  return street;
}

street_t *street_clone(const street_t *street) {
  street_t *clone = malloc(sizeof(street_t));
  if (clone == NULL) {
    return NULL;
  }

  memcpy(clone, street, sizeof(street_t));

  return clone;
}

const char *street_get_ldir(const street_t *street) {
  return street->has_ldir ? street->ldir : NULL;
}

const char *street_get_lesq(const street_t *street) {
  return street->has_lesq ? street->lesq : NULL;
}

double street_get_cmp(const street_t *street) {
  return street->cmp;
}

double street_get_vm(const street_t *street) {
  return street->vm;
}

const char *street_get_nome(const street_t *street) {
  return street->nome;
}

void street_destroy(street_t *street) {
  free(street);
}

void street_set_vm(street_t *street, double vm) {
  street->vm = vm;
}
