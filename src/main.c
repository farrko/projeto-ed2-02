#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datast/exhash.h"
#include "files/geo.h"
#include "files/pm.h"
#include "files/svg.h"
#include "files/qry.h"
#include "objects/block.h"
#include "objects/people.h"
#include "objects/address.h"

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
  char *qrypath = NULL;
  char *pmpath = NULL;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-e") == 0 && i + 1 < argc) base_dir = argv[++i];
    else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) geopath = argv[++i];
    else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) out_dir = argv[++i];
    else if (strcmp(argv[i], "-q") == 0 && i + 1 < argc) qrypath = argv[++i];
    else if (strcmp(argv[i], "-pm") == 0 && i + 1 < argc) pmpath = argv[++i];
  }

  if (geopath == NULL || out_dir == NULL) {
    printf("Uso: ted [-e path] -f arq.geo [-pm arq.pm] [-q arq.qry] -o dir\n");
    exit(1);
  }

  if (base_dir != NULL && base_dir[strlen(base_dir) - 1] == '/') base_dir[strlen(base_dir) - 1] = '\0';
  if (out_dir[strlen(out_dir) - 1] == '/') out_dir[strlen(out_dir) - 1] = '\0';

  char *geo_name = extract_basename(geopath);

  char *full_geopath = malloc(strlen(base_dir != NULL ? base_dir : "") + strlen(geopath) + 2);
  if (base_dir != NULL) sprintf(full_geopath, "%s/%s", base_dir, geopath);
  else strcpy(full_geopath, geopath);

  char *blocks_hfpath = malloc(strlen(out_dir) + strlen(geo_name) + 16);
  sprintf(blocks_hfpath, "%s/%s_blocks", out_dir, geo_name);

  char *people_hfpath = malloc(strlen(out_dir) + strlen(geo_name) + 16);
  sprintf(people_hfpath, "%s/%s_people", out_dir, geo_name);

  char *address_hfpath = malloc(strlen(out_dir) + strlen(geo_name) + 16);
  sprintf(address_hfpath, "%s/%s_addresses", out_dir, geo_name);

  exhash_t *people = exh_init(5, people_sizeof(), people_hfpath);
  exhash_t *addresses = exh_init(5, ads_sizeof(), address_hfpath);
  exhash_t *blocks = exh_init(5, block_sizeof(), blocks_hfpath);

  geo_processing(full_geopath, blocks);

  char *geosvg_path = malloc(strlen(out_dir) + strlen(geo_name) + 8);
  sprintf(geosvg_path, "%s/%s.svg", out_dir, geo_name);

  svg_t *geosvg = svg_init(geosvg_path);
  svg_write_blocks(geosvg, blocks);
  svg_close(geosvg);

  if (pmpath != NULL) {
    char *full_pmpath = malloc(strlen(base_dir != NULL ? base_dir : "") + strlen(pmpath) + 2);
    if (base_dir != NULL) sprintf(full_pmpath, "%s/%s", base_dir, pmpath);
    else strcpy(full_pmpath, pmpath);

    pm_processing(full_pmpath, people, addresses);
    free(full_pmpath);
  }

  if (qrypath == NULL) {
    free(geo_name);
    free(full_geopath);
    free(geosvg_path);
    free(blocks_hfpath);
    free(people_hfpath);
    free(address_hfpath);
    exh_destroy(blocks);
    exh_destroy(people);
    exh_destroy(addresses);
    return 0;
  }

  char *full_qrypath = malloc(strlen(base_dir != NULL ? base_dir : "") + strlen(qrypath) + 2);
  if (base_dir != NULL) sprintf(full_qrypath, "%s/%s", base_dir, qrypath);
  else strcpy(full_qrypath, qrypath);

  char *qry_name = extract_basename(qrypath);

  char *qrysvg_path = malloc(strlen(out_dir) + strlen(geo_name) + strlen(qry_name) + 8);
  sprintf(qrysvg_path, "%s/%s-%s.svg", out_dir, geo_name, qry_name);

  char *qrytxt_path = malloc(strlen(out_dir) + strlen(geo_name) + strlen(qry_name) + 8);
  sprintf(qrytxt_path, "%s/%s-%s.txt", out_dir, geo_name, qry_name);

  svg_t *qrysvg = svg_init(qrysvg_path);

  qry_processing(full_qrypath, qrytxt_path, qrysvg, people, blocks, addresses);

  svg_close(qrysvg);
  exh_destroy(blocks);
  exh_destroy(people);
  exh_destroy(addresses);

  free(geo_name);
  free(qry_name);
  free(full_geopath);
  free(full_qrypath);
  free(geosvg_path);
  free(qrysvg_path);
  free(qrytxt_path);
  free(blocks_hfpath);
  free(people_hfpath);
  free(address_hfpath);

  return 0;
}
