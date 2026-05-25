#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "objects/block.h"

#define CEP_LEN 16
#define COLOR_LEN 16
#define BW_LEN 16

struct block_t {
  char cep[CEP_LEN];
  double x;
  double y;
  double width;
  double height;
  char color[COLOR_LEN];
  char border_color[COLOR_LEN];
  char border_width[BW_LEN];
};

block_t *block_init(const char *cep, double x, double y, double width, double height, const char *color, const char *border_color, const char *border_width) {
  block_t *block = calloc(1, sizeof(block_t));

  strncpy(block->cep, cep, CEP_LEN - 1);
  block->x = x;
  block->y = y;
  block->width = width;
  block->height = height;
  strncpy(block->color, color, COLOR_LEN - 1);
  strncpy(block->border_color, border_color, COLOR_LEN - 1);
  strncpy(block->border_width, border_width, BW_LEN - 1);

  return block;
}

size_t block_sizeof() {
  return sizeof(block_t);
}

const char *block_get_cep(const block_t *block) {
  return block->cep;
}

double block_get_x(const block_t *block) {
  return block->x;
}

double block_get_y(const block_t *block) {
  return block->y;
}

double block_get_width(const block_t *block) {
  return block->width;
}

double block_get_height(const block_t *block) {
  return block->height;
}

const char *block_get_color(const block_t *block) {
  return block->color;
}

const char *block_get_border_color(const block_t *block) {
  return block->border_color;
}

const char *block_get_border_width(const block_t *block) {
  return block->border_width;
}

void block_set_x(block_t *block, double x) {
  block->x = x;
}

void block_set_y(block_t *block, double y) {
  block->y = y;
}

void block_set_width(block_t *block, double w) {
  block->width = w;
}

void block_set_height(block_t *block, double h) {
  block->height = h;
}

void block_set_color(block_t *block, const char *color) {
  strncpy(block->color, color, COLOR_LEN - 1);
  block->color[strnlen(color, COLOR_LEN - 1)] = '\0';
}

void block_set_border_color(block_t *block, const char *border_color) {
  strncpy(block->border_color, border_color, COLOR_LEN - 1);
  block->border_color[strnlen(border_color, COLOR_LEN - 1)] = '\0';
}

void block_set_border_width(block_t *block, const char *border_width) {
  strncpy(block->border_width, border_width, BW_LEN - 1);
  block->border_width[strnlen(border_width, BW_LEN - 1)] = '\0';
}

void block_destroy(block_t *block) {
  free(block);
}
