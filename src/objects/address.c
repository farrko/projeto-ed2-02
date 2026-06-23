#include "objects/address.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

#define CPF_LEN 16
#define CEP_LEN 16
#define COMPL_LEN 32

struct address_t {
  char cep[CEP_LEN];
  char face;
  uint16_t number;
};

address_t *ads_init(const char *cep, char face, uint16_t number) {
  address_t *ads = calloc(1, sizeof(address_t));

  strncpy(ads->cep, cep, CEP_LEN - 1);
  ads->face = face;
  ads->number = number;

  return ads;
}

size_t ads_sizeof() {
  return sizeof(address_t);
}

const char *ads_get_cep(const address_t *ads) {
  return ads->cep;
}

char ads_get_face(const address_t *ads) {
  return ads->face;
}

uint16_t ads_get_number(const address_t *ads) {
  return ads->number;
}

void ads_set_cep(address_t *ads, const char *cep) {
  strncpy(ads->cep, cep, CEP_LEN - 1);
  ads->cep[strnlen(cep, CEP_LEN - 1)] = '\0';
}

void ads_set_face(address_t *ads, char face) {
  ads->face = face;
}

void ads_set_number(address_t *ads, uint16_t number) {
  ads->number = number;
}

void ads_destroy(address_t *ads) {
  free(ads);
}
