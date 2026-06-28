#include "files/qry.h"
#include "files/svg.h"
#include "objects/block.h"
#include "objects/registers.h"
#include "shapes/point.h"
#include "shapes/shapes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static block_t *find_block_by_cep(vector_t *blocks, const char *cep) {
  size_t size = vec_get_size(blocks);

  for (size_t i = 0; i < size; i++) {
    block_t *block = vec_at(blocks, i);
    if (strncmp(block_get_cep(block), cep, 15) == 0) return block;
  }

  return NULL;
}

static point_t *calc_address_pos(char face, int number, block_t *block) {
  double x = block_get_x(block);
  double y = block_get_y(block);

  switch (face) {
    case 'N':
    case 'n':
      x = x + number;
      y = y + block_get_height(block);
      break;
    case 'S':
    case 's':
      x = x + number;
      break;
    case 'L':
    case 'l':
      y = y + number;
      break;
    case 'O':
    case 'o':
      x = x + block_get_width(block);
      y = y + number;
      break;
  }

  return point_init(x, y);
}

static void command_o(int reg, const char *cep, char face, int num, vector_t *blocks, registers_t *registers, FILE *txt, vector_t *added_elements) {
  fprintf(txt, "\n\n--- COMANDO @O? --- argumentos: R%d, %s, %c, %d ---\n\n", reg, cep, face, num);

  block_t *block = find_block_by_cep(blocks, cep);
  if (block == NULL) return;

  point_t *pos = calc_address_pos(face, num, block);
  registers_set(registers, reg, pos);

  double pos_x = point_get_x(pos);
  double pos_y = point_get_y(pos);

  fprintf(txt, "\t- Coordenada: (%.2f, %.2f)", pos_x, pos_y);

  line_t *lx = line_init(0, 0, pos_y, pos_x, pos_y, "#FF0000", true);
  line_t *ly = line_init(0, pos_x, 0, pos_x, pos_y, "#FF0000", true);

  char buf[10];
  sprintf(buf, "R%d", reg);
  text_t *t = text_init(0, pos_x + 10, 10, "start", "#880808", "#000000", "sans-serif", "normal", "8px", buf);

  vec_push_back(added_elements, shape_init(LINE, lx));
  vec_push_back(added_elements, shape_init(LINE, ly));
  vec_push_back(added_elements, shape_init(TEXT, t));
}

void qry_processing(const char *qrypath, const char *txtpath, svg_t *svg, graph_t *graph, vector_t *blocks) {
  (void) svg;
  (void) graph;

  FILE *qry = fopen(qrypath, "r");
  if (qry == NULL) {
    printf("Erro na leitura do arquivo .qry.\n");
    exit(1);
  }

  FILE *txt = fopen(txtpath, "w");
  if (txt == NULL) {
    printf("Erro na criação do arquivo .txt.\n");
    fclose(qry);
    exit(1);
  }

  registers_t *registers = registers_init();
  vector_t *added_elements = vec_init(sizeof(shape_t *));

  char buf[256];
  while (fgets(buf, sizeof(buf), qry)) {
    if (!strncmp(buf, "@o?", 3)) {
      int reg, num;
      char cep[16];
      char face;

      sscanf(buf, "%*s R%d %15s %c %d", &reg, cep, &face, &num);
      command_o(reg, cep, face, num, blocks, registers, txt, added_elements);

      continue;
    }
  }

  registers_destroy(registers);

  fclose(qry);
  fclose(txt);
}
