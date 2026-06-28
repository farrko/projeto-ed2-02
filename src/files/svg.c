#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "files/svg.h"
#include "objects/block.h"
#include "shapes/shapes.h"

struct svg_t {
  FILE *svgfile;
  char path[255];
};

svg_t *svg_init(const char *path) {
  svg_t *svg = calloc(1, sizeof(svg_t));
  if (svg == NULL) {
    printf("Erro na alocação de memória.\n");
    exit(1);
  }

  FILE *file = fopen(path, "w");
  if (file == NULL) {
    printf("Erro na criação do arquivo SVG.\n");
    exit(1);
  }

  fprintf(file, "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"25000\" height=\"25000\">\n");

  svg->svgfile = file;
  strncpy(svg->path, path, 254);

  return svg;
}

void svg_close(svg_t *svg) {
  if (svg == NULL) return;

  FILE *file = svg->svgfile;

  fprintf(file, "</svg>\n");
  fclose(file);
  free(svg);
}

const char *svg_get_path(svg_t *svg) {
  return svg->path;
}

void svg_write_circle(svg_t *svg, circle_t *circle) {
  if (svg == NULL || circle == NULL) return;
  fprintf(svg->svgfile, "<circle id=\"%zu\" cx=\"%f\" cy=\"%f\" r=\"%f\" fill=\"%s\" stroke=\"%s\" fill-opacity=\"0.5\" />\n", circle_get_id(circle), circle_get_x(circle), circle_get_y(circle), circle_get_radius(circle), circle_get_color(circle), circle_get_border_color(circle));
}

void svg_write_rectangle(svg_t *svg, rectangle_t *rect) {
  if (svg == NULL || rect == NULL) return;
  fprintf(svg->svgfile, "<rect id=\"%zu\" x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" fill=\"%s\" stroke=\"%s\" stroke-width=\"%s\" fill-opacity=\"0.5\" />\n", rect_get_id(rect), rect_get_x(rect), rect_get_y(rect), rect_get_width(rect), rect_get_height(rect), rect_get_color(rect), rect_get_border_color(rect), rect_get_border_width(rect));
}

void svg_write_text(svg_t *svg, text_t *text) {
  if (svg == NULL || text == NULL) return;
  fprintf(svg->svgfile, "<text id=\"%zu\" x=\"%f\" y=\"%f\" text-anchor=\"%s\" fill=\"%s\" stroke=\"%s\" font-family=\"%s\" font-weight=\"%s\" font-size=\"%s\" fill-opacity=\"0.5\">\n", text_get_id(text), text_get_x(text), text_get_y(text), text_get_anchor(text), text_get_color(text), text_get_border_color(text), text_get_ffam(text), text_get_fweight(text), text_get_fsize(text));
  fprintf(svg->svgfile, "%s\n", text_get_content(text));
  fprintf(svg->svgfile, "</text>\n");
}

void svg_write_line(svg_t *svg, line_t *line) {
  if (svg == NULL || line == NULL) return;
  fprintf(svg->svgfile, "<line id=\"%zu\" x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke=\"%s\" stroke-dasharray=\"%s\" />\n", line_get_id(line), line_get_x1(line), line_get_y1(line), line_get_x2(line), line_get_y2(line), line_get_color(line), line_get_dotted(line) ? "10" : "none");
}

void svg_write_path(svg_t *svg, path_t *path) {
  if (svg == NULL || path == NULL) return;

  size_t size = path_get_size(path);
  if (size == 0) return;

  point_t *first = path_get_point(path, 0);
  double total_length = 0.0;

  fprintf(svg->svgfile, "<path id=\"%zu\" d=\"M %f,%f", path_get_id(path), point_get_x(first), point_get_y(first));

  for (size_t i = 1; i < size; i++) {
    point_t *prev = path_get_point(path, i - 1);
    point_t *curr = path_get_point(path, i);

    fprintf(svg->svgfile, " L %f,%f", point_get_x(curr), point_get_y(curr));

    total_length += hypot(point_get_x(curr) - point_get_x(prev), point_get_y(curr) - point_get_y(prev));
  }

  fprintf(svg->svgfile, "\" fill=\"none\" stroke=\"%s\" stroke-width=\"2\" stroke-dasharray=\"%f\" stroke-dashoffset=\"%f\">\n", path_get_color(path), total_length, total_length);
  fprintf(svg->svgfile, "<animate attributeName=\"stroke-dashoffset\" from=\"%f\" to=\"0\" dur=\"15s\" fill=\"freeze\" repeatCount=\"indefinite\" />\n", total_length);
  fprintf(svg->svgfile, "</path>\n");
}

void svg_write_blocks(svg_t *svg, vector_t *blocks) {
  size_t n_blocks = vec_get_size(blocks);

  for (size_t i = 0; i < n_blocks; i++) {
    block_t *b = vec_at(blocks, i);

    rectangle_t *rect = rect_init(i, block_get_x(b), block_get_y(b), block_get_width(b), block_get_height(b), block_get_color(b), block_get_border_color(b), block_get_border_width(b));
    svg_write_rectangle(svg, rect);
    rect_destroy(rect);
  }
}

void svg_write_vector_shape(svg_t *svg, vector_t *v) {
  size_t n = vec_get_size(v);

  for (size_t i = 0; i < n; i++) {
    shape_t *s = *(shape_t **) vec_at(v, i);

    switch (shape_get_type(s)) {
      case CIRCLE:
        svg_write_circle(svg, shape_as_circle(s));
        break;
      case RECTANGLE:
        svg_write_rectangle(svg, shape_as_rectangle(s));
        break;
      case TEXT:
        svg_write_text(svg, shape_as_text(s));
        break;
      case LINE:
        svg_write_line(svg, shape_as_line(s));
        break;
      case PATH:
        svg_write_path(svg, shape_as_path(s));
        break;
    }
  }
}
