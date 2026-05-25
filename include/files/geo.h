#ifndef GEO_H

#include "datast/exhash.h"

/*
 *  GEO
 *
 * O módulo GEO tem como função o processamento de um arquivo .geo, que possui informações de
 * quadras em uma cidade. Ele instancia objetos block_t com os dados do arquivo e os registra
 * em um hashfile.
 */

/** @brief    Processa um arquivo .geo e carrega suas informações em um hashfile.
  *
  * @param    geopath  Caminho do arquivo geo.
  * @param    blocks   Estrutura de exhash que irá guardar as informações do .geo.
  */
void geo_processing(char *geopath, exhash_t *blocks);

#endif
