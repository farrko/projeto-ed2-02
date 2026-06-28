#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "shapes/path.h"
#include "datast/vector.h"

struct path_t {
  size_t id;
  vector_t *points;
  char *color;
};

path_t *path_init(size_t id, const char *color) {
  path_t *path = malloc(sizeof(path_t));
  if (path == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  path->id = id;
  path->points = vec_init(sizeof(point_t *));

  char *path_color = malloc(strlen(color) + 1);
  strcpy(path_color, color);
  path->color = path_color;

  return path;
}

void path_destroy(void *path) {
  path_t *p = (path_t *) path;

  size_t size = vec_get_size(p->points);
  for (size_t i = 0; i < size; i++) {
    point_t *point = *(point_t **) vec_at(p->points, i);
    point_destroy(point);
  }

  vec_destroy(p->points);

  if (p->color != NULL) free(p->color);

  free(p);
}

void path_add_point(path_t *path, double x, double y) {
  point_t *point = point_init(x, y);
  vec_push_back(path->points, &point);
}

size_t path_get_id(path_t *path) {
  return path->id;
}

const char *path_get_color(path_t *path) {
  return path->color;
}

size_t path_get_size(path_t *path) {
  return vec_get_size(path->points);
}

point_t *path_get_point(path_t *path, size_t index) {
  point_t **point = vec_at(path->points, index);
  return point != NULL ? *point : NULL;
}
