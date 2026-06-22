#include "files/via.h"
#include "datast/hashmap.h"
#include "objects/street.h"
#include "shapes/point.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static void *point_info_clone(const void *info) {
  point_t *original = (point_t *) info;
  return point_init(point_get_x(original), point_get_y(original));
}

static void point_info_destroy(void *info) {
  point_destroy((point_t *) info);
}

static void *street_info_clone(const void *info) {
  return street_clone((const street_t *) info);
}

static void street_info_destroy(void *info) {
  street_destroy((street_t *) info);
}

graph_t *via_processing(const char *viapath) {
  FILE *via = fopen(viapath, "r");
  if (via == NULL) {
    printf("Erro na leitura do arquivo .via.\n");
    exit(1);
  }

  char buf[256];
  if (fgets(buf, sizeof(buf), via) == NULL) {
    printf("Erro na leitura do arquivo .via.\n");
    fclose(via);
    exit(1);
  }

  graph_t *graph = graph_init(point_info_clone, point_info_destroy, street_info_clone, street_info_destroy);
  bool index_ready = false;

  while (fgets(buf, sizeof(buf), via)) {
    if (!strncmp(buf, "v ", 2)) {
      char id[32];
      double x, y;

      sscanf(buf, "%*s %31s %lf %lf", id, &x, &y);

      point_t *coords = point_init(x, y);
      gnode_t *node = gnode_init(id, coords);

      graph_add_node(graph, node);

      continue;
    }

    if (!strncmp(buf, "e ", 2)) {
      if (!index_ready) {
        graph_generate_node_index(graph);
        index_ready = true;
      }

      char src_id[32], dst_id[32], ldir[16], lesq[16], nome[64];
      double cmp, vm;

      sscanf(buf, "%*s %31s %31s %15s %15s %lf %lf %63s", src_id, dst_id, ldir, lesq, &cmp, &vm, nome);

      hashmap_t *node_index = graph_get_node_index(graph);
      gnode_t *src = hm_get(node_index, src_id);
      gnode_t *dst = hm_get(node_index, dst_id);
      if (src == NULL || dst == NULL) continue;

      street_t *street = street_init(ldir, lesq, cmp, vm, nome);
      graph_add_edge(graph, src, dst, street);
    }
  }

  fclose(via);

  return graph;
}
