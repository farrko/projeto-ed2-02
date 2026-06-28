#ifndef GEO_H

#include "datast/vector.h"

/*
 *  GEO
 *
 * O módulo GEO tem como função o processamento de um arquivo .geo, que possui informações de
 * quadras em uma cidade. Ele instancia objetos block_t com os dados do arquivo e os registra
 * em um vector.
 */

/** @brief    Processa um arquivo .geo e carrega suas informações em um vector_t.
  *
  * @param    geopath  Caminho do arquivo geo.
  *
  * @return   Estrutura de vector com o conteúdo carregado.
  */
vector_t *geo_processing(char *geopath);

#endif
