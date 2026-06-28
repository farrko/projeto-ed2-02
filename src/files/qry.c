#include "files/qry.h"
#include "datast/vector.h"
#include "files/svg.h"
#include "objects/block.h"
#include "objects/registers.h"
#include "shapes/point.h"
#include "shapes/shapes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static hashmap_t *block_vec_to_hm(vector_t *blocks) {
  size_t n = vec_get_size(blocks);

  hashmap_t *hm = hm_init(n > 0 ? n << 1 : 1);

  for (size_t i = 0; i < n; i++) {
    block_t *block = vec_at(blocks, i);
    hm_set(hm, block_get_cep(block), block, NULL);
  }

  return hm;
}

static block_t *find_block_by_cep(hashmap_t *blocks_hm, const char *cep) {
  return hm_get(blocks_hm, cep);
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

static void command_o(int reg, const char *cep, char face, int num, hashmap_t *blocks_hm, registers_t *registers, FILE *txt, vector_t *added_elements) {
  fprintf(txt, "\n\n--- COMANDO @O? --- argumentos: R%d, %s, %c, %d ---\n\n", reg, cep, face, num);

  block_t *block = find_block_by_cep(blocks_hm, cep);
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

  shape_t *shape_lx = shape_init(LINE, lx);
  shape_t *shape_ly = shape_init(LINE, ly);
  shape_t *shape_t_ = shape_init(TEXT, t);

  vec_push_back(added_elements, &shape_lx);
  vec_push_back(added_elements, &shape_ly);
  vec_push_back(added_elements, &shape_t_);
}

static void command_mvm(double v, double x, double y, double w, double h, graph_t *graph, FILE *txt) {
  fprintf(txt, "\n\n--- COMANDO MVM --- argumentos: %.2f, %.2f, %.2f, %.2f, %.2f ---\n\n", v, x, y, w, h);
  (void) graph;
}

static void command_regs(double vl, graph_t *graph, FILE *txt, vector_t *added_elements) {
  fprintf(txt, "\n\n--- COMANDO REGS --- argumentos: %.2f ---\n\n", vl);
  (void) graph;
  (void) added_elements;
}

static void command_exp(double vl, graph_t *graph, FILE *txt, vector_t *added_elements) {
  fprintf(txt, "\n\n--- COMANDO EXP --- argumentos: %.2f ---\n\n", vl);
  (void) graph;
  (void) added_elements;
}

static void command_p(int reg1, int reg2, const char *cc, const char *cr, graph_t *graph, FILE *txt, vector_t *added_elements) {
  fprintf(txt, "\n\n--- COMANDO EXP --- argumentos: R%d, R%d, %s, %s ---\n\n", reg1, reg2, cc, cr);
  (void) graph;
  (void) added_elements;
}

void qry_processing(const char *qrypath, const char *txtpath, svg_t *svg, graph_t *graph, vector_t *blocks) {
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
  vector_t *added_elements = vec_init(sizeof(shape_t **));
  hashmap_t *blocks_hm = block_vec_to_hm(blocks);

  char buf[256];
  while (fgets(buf, sizeof(buf), qry)) {
    if (!strncmp(buf, "@o?", 3)) {
      int reg, num;
      char cep[16];
      char face;

      sscanf(buf, "%*s R%d %15s %c %d", &reg, cep, &face, &num);
      command_o(reg, cep, face, num, blocks_hm, registers, txt, added_elements);

      continue;
    }

    if (!strncmp(buf, "mvm", 3)) {
      double v, x, y, w, h;

      sscanf(buf, "%*s %lf %lf %lf %lf %lf", &v, &x, &y, &w, &h);
      command_mvm(v, x, y, w, h, graph, txt);

      continue;
    }

    if (!strncmp(buf, "regs", 4)) {
      double vl;

      sscanf(buf, "%*s %lf", &vl);
      command_regs(vl, graph, txt, added_elements);

      continue;
    }

    if (!strncmp(buf, "exp", 3)) {
      double vl;

      sscanf(buf, "%*s %lf", &vl);
      command_exp(vl, graph, txt, added_elements);

      continue;
    }

    if (!strncmp(buf, "p?", 2)) {
      int reg1, reg2;
      char cc[16], cr[16];

      sscanf(buf, "%*s R%d R%d %s %s", &reg1, &reg2, cc, cr);
      command_p(reg1, reg2, cc, cr, graph, txt, added_elements);

      continue;
    }
  }

  svg_write_vector_shape(svg, added_elements);
  size_t n = vec_get_size(added_elements);
  for (size_t i = 0; i < n; i++) shape_destroy(*(shape_t **) vec_at(added_elements, i));
  vec_destroy(added_elements);

  registers_destroy(registers);
  hm_destroy(blocks_hm);

  fclose(qry);
  fclose(txt);
}
