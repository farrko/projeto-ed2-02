#include "files/geo.h"

#include <stdio.h>
#include <string.h>
#include "objects/block.h"

vector_t *geo_processing(char *geopath) {
  FILE *geo = fopen(geopath, "r");
  vector_t *blocks = vec_init(block_sizeof());

  char border_width[16] = "2px";
  char color[16] = "#0F0F0F";
  char border_color[16] = "#DF3F3F";

  char buf[255];
  while(fgets(buf, 255, geo)) {
    if (!strncmp(buf, "q", 1)) {
      char cep[16];
      double x, y, width, height;

      sscanf(buf, "%*s %s %lf %lf %lf %lf", cep, &x, &y, &width, &height);

      block_t *block = block_init(cep, x, y, width, height, color, border_color, border_width);
      vec_push_back(blocks, block);
      block_destroy(block);
    }

    if (!strncmp(buf, "cq", 2)) {
      sscanf(buf, "%*s %s %s %s", border_width, color, border_color);
    }
  }

  fclose(geo);
  return blocks;
}
