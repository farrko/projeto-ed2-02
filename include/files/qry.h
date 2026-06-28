#ifndef QRY_H
#define QRY_H

#include "svg.h"
#include "datast/vector.h"
#include "datast/graphs.h"

/*
 *  PROCESSAMENTO DE CONSULTAS
 *
 * O módulo de consultas (qry) é responsável por processar um arquivo de consultas e produzir
 * os resultados correspondentes em um arquivo de texto e em um arquivo SVG. As consultas
 * operam sobre o grafo do sistema viário e o vetor de quadras previamente carregados, podendo
 * gerar saídas textuais e representações visuais das operações realizadas.
 */

/** @brief   Processa um arquivo de consultas, gerando saídas textual e visual.
 *
 * @param   qrypath  Caminho do arquivo de consultas a ser processado.
 * @param   txtpath  Caminho do arquivo de texto para escrita dos resultados.
 * @param   svg      Instância SVG para escrita das representações visuais.
 * @param   graph    Grafo do sistema viário, previamente carregado a partir de um arquivo
 *                   .via.
 * @param   blocks   Vetor de quadras (block_t), previamente carregado a partir de um arquivo
 *                   .geo.
 */
void qry_processing(const char *qrypath, const char *txtpath, svg_t *svg, graph_t *graph, vector_t *blocks);

#endif
