#include <stdlib.h>
#include <stdio.h>

#include "shapes/shapes.h"

struct shape_t {
  E_SHAPETYPE shapetype;
  void *shape;
};

shape_t *shape_init(E_SHAPETYPE type, void *shape) {
  shape_t *s = malloc(sizeof(shape_t));
  if (s == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  s->shapetype = type;
  s->shape = shape;

  return s;
}

void shape_destroy(void *shape) {
  shape_t *s = (shape_t *) shape;

   switch(s->shapetype) {
    case CIRCLE:
      circle_destroy(s->shape);
      break;
    case RECTANGLE:
      rect_destroy(s->shape);
      break;
    case TEXT:
      text_destroy(s->shape);
      break;
  }

  free(shape);
}

E_SHAPETYPE shape_get_type(shape_t *shape) {
  return shape->shapetype;
}

circle_t *shape_as_circle(shape_t *shape) {
  return (circle_t *) shape->shape;
}

rectangle_t *shape_as_rectangle(shape_t *shape) {
  return (rectangle_t *) shape->shape;
}

text_t *shape_as_text(shape_t *shape) {
  return (text_t *) shape->shape;
}

node_t *shape_as_node(shape_t *shape) {
  return node_init(shape, shape_destroy);
}
