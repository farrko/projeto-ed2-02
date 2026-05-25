#ifndef QRY_H
#define QRY_H

#include "svg.h"
#include "datast/exhash.h"

/*
 *  PROCESSAMENTO DE CONSULTAS
 *
 * O módulo de consultas (qry) é responsável por processar um arquivo de consultas e produzir
 * os resultados correspondentes em um arquivo de texto e em um arquivo SVG. As consultas
 * operam sobre as tabelas de pessoas, quadras e endereços previamente carregadas, podendo
 * gerar saídas textuais e representações visuais das operações realizadas.
 */

/** @brief   Processa um arquivo de consultas, gerando saídas textual e visual.
 *
 * @param   qrypath    Caminho do arquivo de consultas a ser processado.
 * @param   txtpath    Caminho do arquivo de texto para escrita dos resultados.
 * @param   svg        Instância SVG para escrita das representações visuais.
 * @param   people     Tabela de hashing contendo os registros de pessoas.
 * @param   blocks     Tabela de hashing contendo os registros de quadras.
 * @param   addresses  Tabela de hashing contendo os registros de endereços.
 */
void qry_processing(const char *qrypath, const char *txtpath, svg_t *svg, exhash_t *people, exhash_t *blocks, exhash_t *addresses);

#endif
