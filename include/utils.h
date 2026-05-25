#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

/*
 *  UTILITÁRIOS
 *
 * Este módulo reúne funções utilitárias de uso geral, sem vínculo com nenhum módulo específico
 * do sistema. As funções aqui presentes complementam a biblioteca padrão do C com operações
 * recorrentes que não são nativamente disponibilizadas por ela.
 */

/** @brief   Concatena duas strings em uma nova região de memória.
 *
 * @param   s1  Primeira string.
 * @param   s2  Segunda string.
 *
 * @return  Ponteiro para uma nova string contendo a concatenação de s1 e s2.
 */
char *r_strcat(char *s1, char *s2);

/** @brief   Retorna o comprimento de uma string, limitado a um número máximo de caracteres.
 *
 * @param   str     String a ser medida.
 * @param   maxlen  Número máximo de caracteres a percorrer.
 *
 * @return  O comprimento da string, ou maxlen caso o terminador nulo não seja
 *          encontrado dentro dos primeiros maxlen caracteres.
 */
size_t strnlen(const char *str, size_t maxlen);

#endif
