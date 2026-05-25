#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "files/svg.h"
#include "objects/block.h"

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

void svg_write_blocks(svg_t *svg, exhash_t *blocks) {
  uint16_t n_blocks = exh_entries_amount(blocks);
  uint8_t *block_list = exh_get_all(blocks);

  for (uint16_t i = 0; i < n_blocks; i++) {
    block_t *b = (block_t *)(block_list + i * block_sizeof());

    rectangle_t *rect = rect_init(i, block_get_x(b), block_get_y(b), block_get_width(b), block_get_height(b), block_get_color(b), block_get_border_color(b), block_get_border_width(b));
    svg_write_rectangle(svg, rect);
    rect_destroy(rect);
  }

  free(block_list);
}
