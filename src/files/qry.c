#include "files/qry.h"
#include "datast/graphs.h"
#include "datast/vector.h"
#include "files/svg.h"
#include "objects/block.h"
#include "objects/registers.h"
#include "shapes/line.h"
#include "shapes/point.h"
#include "shapes/shapes.h"
#include "objects/street.h"
#include "shapes/rectangle.h"

#include <math.h>
#include <stddef.h>
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

static gnode_t *graph_add_intermediary_node(graph_t *graph, point_t *point, const char *key) {
  vector_t *graph_vec = graph_get_nodes(graph);
  size_t graph_n = vec_get_size(graph_vec);

  for (size_t i = 0; i < graph_n; i++) {
    gnode_t *node = *(gnode_t **) vec_at(graph_vec, i);

    vector_t *conn_vec = gnode_get_connections(node);
    size_t conn_n = vec_get_size(conn_vec);

    for (size_t j = 0; j < conn_n; j++) {
      edge_t *edge = *(edge_t **) vec_at(conn_vec, j);
      gnode_t *src = edge_get_src(edge);
      gnode_t *dst = edge_get_dst(edge);

      point_t *src_pos = gnode_get_info(src);
      point_t *dst_pos = gnode_get_info(dst);

      if (point_get_x(src_pos) == point_get_x(point) && point_get_y(src_pos) == point_get_y(point)) continue;
      if (point_get_x(dst_pos) == point_get_x(point) && point_get_y(dst_pos) == point_get_y(point)) continue;

      line_t *l = line_init(0, point_get_x(src_pos), point_get_y(src_pos), point_get_x(dst_pos), point_get_y(dst_pos), "#000000", false);
      bool proximity = line_within_proximity_to_point(l, point, 15.0);
      line_destroy(l);
      if (!proximity) continue;

      gnode_t *newly_added = gnode_init(key, point_clone(point));
      graph_add_node(graph, newly_added);

      street_t *edge_street = edge_get_info(edge);
      double distance_to_src = sqrt(point_calculate_distance_squared(point, gnode_get_info(src)));
      double distance_to_dst = sqrt(point_calculate_distance_squared(point, gnode_get_info(dst)));

      street_t *to_src = street_init(street_get_ldir(edge_street), street_get_lesq(edge_street), distance_to_src, street_get_vm(edge_street), street_get_nome(edge_street));
      street_t *from_src = street_clone(to_src);
      street_t *to_dst = street_init(street_get_ldir(edge_street), street_get_lesq(edge_street), distance_to_dst, street_get_vm(edge_street), street_get_nome(edge_street));
      street_t *from_dst = street_clone(to_dst);

      graph_add_edge(graph, newly_added, src, to_src);
      graph_add_edge(graph, src, newly_added, from_src);
      graph_add_edge(graph, newly_added, dst, to_dst);
      graph_add_edge(graph, dst, newly_added, from_dst);

      point_destroy(point);
      return newly_added;
    }
  }

  return NULL;
}

static double fastest_edge(edge_t *edge) {
  street_t *street = edge_get_info(edge);
  return street_get_cmp(street) / street_get_vm(street);
}

static double shortest_edge(edge_t *edge) {
  street_t *street = edge_get_info(edge);
  return street_get_cmp(street);
}

static double pure_speed_edge(edge_t *edge) {
  street_t *street = edge_get_info(edge);
  return street_get_vm(street);
}

static int edge_cmp(const void *e1, const void *e2) {
  edge_t *edge1 = *(edge_t * const *) e1;
  edge_t *edge2 = *(edge_t * const *) e2;

  double s1 = shortest_edge(edge1);
  double s2 = shortest_edge(edge2);

  if (s1 < s2) return -1;
  if (s1 > s2) return 1;
  return 0;
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

  rectangle_t *box = rect_init(0, x, y, w, h, "#000000", "#000000", "2px");

  vector_t *graph_vec = graph_get_nodes(graph);
  size_t graph_n = vec_get_size(graph_vec);

  for (size_t i = 0; i < graph_n; i++) {
    gnode_t *node = *(gnode_t **) vec_at(graph_vec, i);

    vector_t *conn_vec = gnode_get_connections(node);
    size_t conn_n = vec_get_size(conn_vec);

    for (size_t j = 0; j < conn_n; j++) {
      edge_t *edge = *(edge_t **) vec_at(conn_vec, j);
      gnode_t *src = edge_get_src(edge);
      gnode_t *dst = edge_get_dst(edge);

      point_t *src_pos = gnode_get_info(src);
      point_t *dst_pos = gnode_get_info(dst);

      line_t *l = line_init(0, point_get_x(src_pos), point_get_y(src_pos), point_get_x(dst_pos), point_get_y(dst_pos), "#000000", false);

      bool overlap = rect_line_overlap(box, l);
      if (overlap) {
        street_t *street = edge_get_info(edge);
        double prev_vm = street_get_vm(street);
        street_set_vm(street, v);
        fprintf(txt, "\tEdge modificada: %s -> %s - Velocidade anterior: %.2f / Velocidade atual: %.2f\n", gnode_get_id(edge_get_src(edge)), gnode_get_id(edge_get_dst(edge)), prev_vm, street_get_vm(street));
      }
      line_destroy(l);
    }
  }

  rect_destroy(box);
}

static int tarjan_cmp(const void *a, const void *b) {
  double weight = *(double *) a;
  double limiter = *(double *) b;

  if (weight < limiter) return 1;
  return -1;
}

static void command_regs(double vl, graph_t *graph, FILE *txt, vector_t *added_elements) {
  fprintf(txt, "\n\n--- COMANDO REGS --- argumentos: %.2f ---\n\n", vl);

  vector_t *components = graph_tarjan(graph, tarjan_cmp, pure_speed_edge, vl);
  size_t components_n = vec_get_size(components);
  fprintf(txt, "\tQuantidade de componentes fortemente conexos: %zu\n", components_n);

  const char* bb_colors[32] = { "#E31A1C", "#20DB93", "#A020F0", "#FF7F00", "#1F78B4", "#FFFF33", "#FB9A99", "#33A02C", "#B2DF8A", "#A6CEE3", "#E6AB02", "#7570B3", "#66A61E", "#E7298A", "#1B9E77", "#D95F02", "#8DD3C7", "#FFFFB3", "#BEBADA", "#FB8072", "#80B1D3", "#FDB462", "#B3DE69", "#FCCDE5", "#BC80BD", "#CCEBC5", "#FFED6F", "#1F77B4", "#AEC7E8", "#FF7F0E", "#FFBB78", "#2CA02C" };

  for (size_t i = 0; i < components_n; i++) {
    graph_t *cg = *(graph_t **) vec_at(components, i);
    size_t cg_n = vec_get_size(graph_get_nodes(cg));

    double min_x = INFINITY, min_y = INFINITY, max_x = -INFINITY, max_y = -INFINITY;

    for (size_t j = 0; j < cg_n; j++) {
      gnode_t *node = *(gnode_t **) vec_at(graph_get_nodes(cg), j);
      point_t *point = gnode_get_info(node);

      if (point_get_x(point) < min_x) min_x = point_get_x(point);
      if (point_get_x(point) > max_x) max_x = point_get_x(point);
      if (point_get_y(point) < min_y) min_y = point_get_y(point);
      if (point_get_y(point) > max_y) max_y = point_get_y(point);
    }

    rectangle_t *bounding_box = rect_init(0, min_x - 15, min_y - 15, max_x - min_x + 15, max_y - min_y + 15, bb_colors[i % 32], bb_colors[i % 32], "2px");
    shape_t *s = shape_init(RECTANGLE, bounding_box);
    vec_push_back(added_elements, &s);

    graph_destroy(cg);
  }

  vec_destroy(components);
}

static void command_exp(double vl, graph_t *graph, FILE *txt, vector_t *added_elements) {
  fprintf(txt, "\n\n--- COMANDO EXP --- argumentos: %.2f ---\n\n", vl);

  graph_t *undirected = graph_to_undirected(graph, true);
  graph_t *mst = graph_kruskal(undirected, edge_cmp, pure_speed_edge, vl);

  vector_t *nodes = graph_get_nodes(mst);
  size_t nodes_size = vec_get_size(nodes);
  for (size_t i = 0; i < nodes_size; i++) {
    gnode_t *node = *(gnode_t **) vec_at(nodes, i);
    vector_t *conns = gnode_get_connections(node);
    size_t conn_size = vec_get_size(conns);

    for (size_t j = 0; j < conn_size; j++) {
      edge_t *edge = *(edge_t **) vec_at(conns, j);
      street_t *st = edge_get_info(edge);
      street_set_vm(st, street_get_vm(st) * 1.5);

      point_t *src_point = gnode_get_info(edge_get_src(edge));
      point_t *dst_point = gnode_get_info(edge_get_dst(edge));

      line_t *l = line_init(0, point_get_x(src_point), point_get_y(src_point), point_get_x(dst_point), point_get_y(dst_point), "#780606", false);
      shape_t *s = shape_init(LINE, l);

      vec_push_back(added_elements, &s);
    }
  }

  graph_destroy(mst);
  graph_destroy(undirected);
}

static void command_p(int reg1, int reg2, const char *cc, const char *cr, registers_t *registers, graph_t *graph, FILE *txt, vector_t *added_elements) {
  fprintf(txt, "\n\n--- COMANDO P? --- argumentos: R%d, R%d, %s, %s ---\n\n", reg1, reg2, cc, cr);

  graph_t *clone = graph_clone(graph);

  gnode_t *start = graph_add_intermediary_node(clone, point_clone(registers_get(registers, reg1)), "dijkstra_start");
  gnode_t *end = graph_add_intermediary_node(clone, point_clone(registers_get(registers, reg2)), "dijkstra_end");
  graph_generate_node_index(clone);

  circle_t *sc = circle_init(0, point_get_x(gnode_get_info(start)), point_get_y(gnode_get_info(start)), 10, "##A3CF05", "#A3CF05");
  text_t *st = text_init(0, point_get_x(gnode_get_info(start)), point_get_y(gnode_get_info(start)), "middle", "#FFFFFF", "#000000", "sans-serif", "normal", "8px", "I");
  circle_t *ec = circle_init(0, point_get_x(gnode_get_info(end)), point_get_y(gnode_get_info(end)), 10, "#E53A20", "#E53A20");
  text_t *et = text_init(0, point_get_x(gnode_get_info(end)), point_get_y(gnode_get_info(end)), "middle", "#FFFFFF", "#000000", "sans-serif", "normal", "8px", "F");

  shape_t *shape_sc = shape_init(CIRCLE, sc);
  shape_t *shape_st = shape_init(TEXT, st);
  shape_t *shape_ec = shape_init(CIRCLE, ec);
  shape_t *shape_et = shape_init(TEXT, et);

  vec_push_back(added_elements, &shape_sc);
  vec_push_back(added_elements, &shape_st);
  vec_push_back(added_elements, &shape_ec);
  vec_push_back(added_elements, &shape_et);

  vector_t *shortest_dijk = graph_dijkstra(clone, start, end, shortest_edge);
  size_t sdn = vec_get_size(shortest_dijk);
  vector_t *fastest_dijk = graph_dijkstra(clone, start, end, fastest_edge);
  size_t fdn = vec_get_size(fastest_dijk);

  if (dijc_get_cost(vec_at(shortest_dijk, 0)) == INFINITY) {
    fprintf(txt, "\t- Distância entre começo e fim infinita: não há conexão entre os dois pontos.\n");

    vec_destroy(shortest_dijk);
    vec_destroy(fastest_dijk);

    graph_destroy(clone);

    return;
  }

  path_t *shortest_path = path_init(0, cc);
  path_add_point(shortest_path, point_get_x(gnode_get_info(dijc_get_from(vec_at(shortest_dijk, 0)))), point_get_y(gnode_get_info(dijc_get_from(vec_at(shortest_dijk, 0)))));
  fprintf(txt, " - CAMINHO MAIS CURTO\n");
  fprintf(txt, "  0. Ponto (%.2f, %.2f) - Origem\n", point_get_x(gnode_get_info(dijc_get_from(vec_at(shortest_dijk, 0)))), point_get_y(gnode_get_info(dijc_get_from(vec_at(shortest_dijk, 0)))));
  for (size_t i = 0; i < sdn; i++) {
    size_t n = i + 1;
    if (n == sdn) fprintf(txt, "  %zu. Ponto (%.2f, %.2f) - Destino\n", n, point_get_x(gnode_get_info(dijc_get_node(vec_at(shortest_dijk, i)))), point_get_y(gnode_get_info(dijc_get_node(vec_at(shortest_dijk, i)))));
    else fprintf(txt, "  %zu. Ponto (%.2f, %.2f)\n", n, point_get_x(gnode_get_info(dijc_get_node(vec_at(shortest_dijk, i)))), point_get_y(gnode_get_info(dijc_get_node(vec_at(shortest_dijk, i)))));

    path_add_point(shortest_path, point_get_x(gnode_get_info(dijc_get_node(vec_at(shortest_dijk, i)))), point_get_y(gnode_get_info(dijc_get_node(vec_at(shortest_dijk, i)))));
  }

  path_t *fastest_path = path_init(0, cr);
  path_add_point(fastest_path, point_get_x(gnode_get_info(dijc_get_from(vec_at(fastest_dijk, 0)))), point_get_y(gnode_get_info(dijc_get_from(vec_at(fastest_dijk, 0)))));
  fprintf(txt, " - CAMINHO MAIS RÁPIDO\n");
  fprintf(txt, "  0. Ponto (%.2f, %.2f) - Origem\n", point_get_x(gnode_get_info(dijc_get_from(vec_at(fastest_dijk, 0)))), point_get_y(gnode_get_info(dijc_get_from(vec_at(fastest_dijk, 0)))));
  for (size_t i = 0; i < fdn; i++) {
    size_t n = i + 1;
    if (n == fdn) fprintf(txt, "  %zu. Ponto (%.2f, %.2f) - Destino\n", n, point_get_x(gnode_get_info(dijc_get_node(vec_at(fastest_dijk, i)))), point_get_y(gnode_get_info(dijc_get_node(vec_at(fastest_dijk, i)))));
    else fprintf(txt, "  %zu. Ponto (%.2f, %.2f)\n", n, point_get_x(gnode_get_info(dijc_get_node(vec_at(fastest_dijk, i)))), point_get_y(gnode_get_info(dijc_get_node(vec_at(fastest_dijk, i)))));

    path_add_point(fastest_path, point_get_x(gnode_get_info(dijc_get_node(vec_at(fastest_dijk, i)))), point_get_y(gnode_get_info(dijc_get_node(vec_at(fastest_dijk, i)))));
  }

  shape_t *shape_sp = shape_init(PATH, shortest_path);
  shape_t *shape_fp = shape_init(PATH, fastest_path);

  vec_push_back(added_elements, &shape_sp);
  vec_push_back(added_elements, &shape_fp);

  vec_destroy(shortest_dijk);
  vec_destroy(fastest_dijk);

  graph_destroy(clone);
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
      command_p(reg1, reg2, cc, cr, registers, graph, txt, added_elements);

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
