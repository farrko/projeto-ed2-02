#include "files/geo.h"

#include <stdio.h>
#include <string.h>
#include "datast/exhash.h"
#include "objects/block.h"

void geo_processing(char *geopath, exhash_t *blocks) {
  FILE *geo = fopen(geopath, "r");

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
      exh_insert(blocks, cep, block);

      block_destroy(block);
    }

    if (!strncmp(buf, "cq", 2)) {
      sscanf(buf, "%*s %s %s %s", border_width, color, border_color);
    }
  }

  fclose(geo);
}
