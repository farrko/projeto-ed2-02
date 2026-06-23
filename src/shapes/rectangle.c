#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "shapes/rectangle.h"

struct rectangle_t {
  size_t id;
  point_t *origin;
  double width, height;
  char color[16];
  char border_color[16];
  char border_width[16];
};

rectangle_t *rect_init(size_t id, double x, double y, double width, double height, const char *color, const char *border_color, const char *border_width) {
  rectangle_t *rectangle = calloc(1, sizeof(rectangle_t));
  if (rectangle == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  rectangle->id = id;

  point_t *origin = point_init(x, y);
  rectangle->origin = origin;

  rectangle->width = width;
  rectangle->height = height;

  strncpy(rectangle->color, color, 15);
  strncpy(rectangle->border_color, border_color, 15);
  strncpy(rectangle->border_width, border_width, 15);

  return rectangle;
}

void rect_destroy(void *rectangle) {
  rectangle_t *rect = (rectangle_t *) rectangle;

  point_destroy(rect->origin);

  free(rect);
}

void rect_set_x(rectangle_t *rect, double x) {
  point_set_x(rect->origin, x);
}

void rect_set_y(rectangle_t *rect, double y) {
  point_set_y(rect->origin, y);
}

void rect_set_width(rectangle_t *rect, double width) {
  rect->width = width;
}

void rect_set_height(rectangle_t *rect, double height) {
  rect->height = height;
}

void rect_set_color(rectangle_t *rect, const char *color) {
  strncpy(rect->color, color, 15);
}

void rect_set_border_color(rectangle_t *rect, const char *border_color) {
  strncpy(rect->border_color, border_color, 15);
}

void rect_set_border_width(rectangle_t *rect, const char *border_width) {
  strncpy(rect->border_width, border_width, 15);
}

size_t rect_get_id(rectangle_t *rect) {
  return rect->id;
}

point_t *rect_get_origin(rectangle_t *rect) {
  return rect->origin;
}

double rect_get_x(rectangle_t *rect) {
  return point_get_x(rect->origin);
}

double rect_get_y(rectangle_t *rect) {
  return point_get_y(rect->origin);
}

double rect_get_width(rectangle_t *rect) {
  return rect->width;
}

double rect_get_height(rectangle_t *rect) {
  return rect->height;
}

const char *rect_get_color(rectangle_t *rect) {
  return rect->color;
}

const char *rect_get_border_color(rectangle_t *rect) {
  return rect->border_color;
}

const char *rect_get_border_width(rectangle_t *rect) {
  return rect->border_width;
}
