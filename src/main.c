#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datast/vector.h"
#include "datast/graphs.h"
#include "files/geo.h"
#include "files/via.h"
#include "files/svg.h"
#include "files/qry.h"

static char *extract_basename(const char *filepath) {
  const char *slash = strrchr(filepath, '/');
  const char *start = (slash != NULL) ? slash + 1 : filepath;

  const char *dot = strrchr(start, '.');
  size_t len = (dot != NULL) ? (size_t)(dot - start) : strlen(start);

  char *name = malloc(len + 1);
  strncpy(name, start, len);
  name[len] = '\0';

  return name;
}

int main(int argc, char **argv) {
  char *base_dir = NULL;
  char *geopath = NULL;
  char *out_dir = NULL;
  char *viapath = NULL;
  char *qrypath = NULL;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-e") == 0 && i + 1 < argc) base_dir = argv[++i];
    else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) geopath = argv[++i];
    else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) out_dir = argv[++i];
    else if (strcmp(argv[i], "-v") == 0 && i + 1 < argc) viapath = argv[++i];
    else if (strcmp(argv[i], "-q") == 0 && i + 1 < argc) qrypath = argv[++i];
  }

  if (geopath == NULL || out_dir == NULL) {
    printf("Uso: ted [-e path] -f arq.geo [-v arq.via] [-q arq.qry] -o dir\n");
    exit(1);
  }

  if (base_dir != NULL && base_dir[strlen(base_dir) - 1] == '/') base_dir[strlen(base_dir) - 1] = '\0';
  if (out_dir[strlen(out_dir) - 1] == '/') out_dir[strlen(out_dir) - 1] = '\0';

  char *geo_name = extract_basename(geopath);

  char *full_geopath = malloc(strlen(base_dir != NULL ? base_dir : "") + strlen(geopath) + 2);
  if (base_dir != NULL) sprintf(full_geopath, "%s/%s", base_dir, geopath);
  else strcpy(full_geopath, geopath);

  vector_t *blocks = geo_processing(full_geopath);

  char *geosvg_path = malloc(strlen(out_dir) + strlen(geo_name) + 8);
  sprintf(geosvg_path, "%s/%s.svg", out_dir, geo_name);

  svg_t *geosvg = svg_init(geosvg_path);
  svg_write_blocks(geosvg, blocks);
  svg_close(geosvg);

  graph_t *graph = NULL;
  char *full_viapath = NULL;

  if (viapath != NULL) {
    full_viapath = malloc(strlen(base_dir != NULL ? base_dir : "") + strlen(viapath) + 2);
    if (base_dir != NULL) sprintf(full_viapath, "%s/%s", base_dir, viapath);
    else strcpy(full_viapath, viapath);

    graph = via_processing(full_viapath);
  }

  if (qrypath != NULL) {
    char *full_qrypath = malloc(strlen(base_dir != NULL ? base_dir : "") + strlen(qrypath) + 2);
    if (base_dir != NULL) sprintf(full_qrypath, "%s/%s", base_dir, qrypath);
    else strcpy(full_qrypath, qrypath);

    char *qry_name = extract_basename(qrypath);

    char *qrysvg_path = malloc(strlen(out_dir) + strlen(geo_name) + strlen(qry_name) + 8);
    sprintf(qrysvg_path, "%s/%s-%s.svg", out_dir, geo_name, qry_name);

    char *qrytxt_path = malloc(strlen(out_dir) + strlen(geo_name) + strlen(qry_name) + 8);
    sprintf(qrytxt_path, "%s/%s-%s.txt", out_dir, geo_name, qry_name);

    svg_t *qrysvg = svg_init(qrysvg_path);

    svg_write_blocks(qrysvg, blocks);
    qry_processing(full_qrypath, qrytxt_path, qrysvg, graph, blocks);

    svg_close(qrysvg);

    free(qry_name);
    free(full_qrypath);
    free(qrysvg_path);
    free(qrytxt_path);
  }

  vec_destroy(blocks);
  if (graph != NULL) graph_destroy(graph);

  free(geo_name);
  free(full_geopath);
  free(geosvg_path);
  free(full_viapath);

  return 0;
}
