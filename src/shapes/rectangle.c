#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "shapes/rectangle.h"
#include "shapes/line.h"

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

static bool ccw(double Ax, double Ay, double Bx, double By, double Cx, double Cy) {
    return (Cy - Ay) * (Bx - Ax) > (By - Ay) * (Cx - Ax);
}

static bool line_line_overlap(line_t *l1, line_t *l2) {
  double l1x1 = line_get_x1(l1);
  double l1y1 = line_get_y1(l1);
  double l1x2 = line_get_x2(l1);
  double l1y2 = line_get_y2(l1);

  double l2x1 = line_get_x1(l2);
  double l2y1 = line_get_y1(l2);
  double l2x2 = line_get_x2(l2);
  double l2y2 = line_get_y2(l2);

  bool cond1 = ccw(l1x1, l1y1, l2x1, l2y1, l2x2, l2y2);
  bool cond2 = ccw(l1x2, l1y2, l2x1, l2y1, l2x2, l2y2);
  bool cond3 = ccw(l1x1, l1y1, l1x2, l1y2, l2x1, l2y1);
  bool cond4 = ccw(l1x1, l1y1, l1x2, l1y2, l2x2, l2y2);

  return (cond1 != cond2) && (cond3 != cond4);
}

bool rect_line_overlap(rectangle_t *r, line_t *l) {
  double rx = rect_get_x(r);
  double ry = rect_get_y(r);
  double rwidth = rect_get_width(r);
  double rheight = rect_get_height(r);

  double lx1 = line_get_x1(l);
  double ly1 = line_get_y1(l);
  double lx2 = line_get_x2(l);
  double ly2 = line_get_y2(l);

  // Checa se existe um ponto de L dentro de R
  bool inside1 = (lx1 >= rx && lx1 <= rx + rwidth && ly1 >= ry && ly1 <= ry + rheight);
  bool inside2 = (lx2 >= rx && lx2 <= rx + rwidth && ly2 >= ry && ly2 <= ry + rheight);
  if (inside1 || inside2) return true;

  // Criação de linhas em cada aresta do retângulo, para usar a line_line_overlap para verificar se existe sobreposição
  line_t *top_edge = line_init(0, rx, ry, rx + rwidth, ry, "#000000", false);
  line_t *left_edge = line_init(0, rx, ry, rx, ry + rheight, "#000000", false);
  line_t *right_edge = line_init(0, rx + rwidth, ry, rx + rwidth, ry + rheight, "#000000", false);
  line_t *bottom_edge = line_init(0, rx, ry + rheight, rx + rwidth, ry + rheight, "#000000", false);

  bool tl = line_line_overlap(l, top_edge);
  bool ll = line_line_overlap(l, left_edge);
  bool rl = line_line_overlap(l, right_edge);
  bool bl = line_line_overlap(l, bottom_edge);

  line_destroy(top_edge);
  line_destroy(left_edge);
  line_destroy(right_edge);
  line_destroy(bottom_edge);

  if (tl || ll || rl || bl) return true;

  return false;
}
