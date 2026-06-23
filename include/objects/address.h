#ifndef ADDRESS_H
#define ADDRESS_H

#include <stddef.h>
#include <stdint.h>

/*
 *  ENDEREÇO
 *
 * O módulo de endereço (address) define a estrutura address_t, responsável por representar
 * um endereço qualquer. Cada endereço é definido com CEP, face (lado da rua), número e complemento.
 *
 * A estrutura pode ser criada por meio de ads_init() e destruída com ads_destroy().
 * São disponibilizadas operações de leitura (get) e escrita (set) para cada campo.
 */

typedef struct address_t address_t;

/** @brief   Inicializa uma nova estrutura de endereço com os dados fornecidos.
 *
 * @param   cep         CEP do endereço.
 * @param   face        Face (lado da rua) do endereço.
 * @param   number      Número do endereço.
 *
 * @return  Ponteiro para a nova estrutura address_t inicializada.
 */
address_t *ads_init(const char *cep, char face, uint16_t number);

/** @brief   Retorna o tamanho em bytes da estrutura address_t.
 *
 * @return  Tamanho em bytes de address_t.
 */
size_t ads_sizeof();

/** @brief   Recupera o CEP do endereço.
 *
 * @param   a  Ponteiro para o endereço.
 *
 * @return  Ponteiro para a string contendo o CEP.
 */
const char *ads_get_cep(const address_t *a);

/** @brief   Recupera a face (lado da rua) do endereço.
 *
 * @param   a  Ponteiro para o endereço.
 *
 * @return  Caractere representando a face do endereço.
 */
char ads_get_face(const address_t *a);

/** @brief   Recupera o número do endereço.
 *
 * @param   a  Ponteiro para o endereço.
 *
 * @return  Número do endereço.
 */
uint16_t ads_get_number(const address_t *a);

/** @brief   Define o CEP do endereço.
 *
 * @param   a    Ponteiro para o endereço.
 * @param   cep  Nova string de CEP a ser atribuída.
 */
void ads_set_cep(address_t *a, const char *cep);

/** @brief   Define a face (lado da rua) do endereço.
 *
 * @param   a     Ponteiro para o endereço.
 * @param   face  Novo caractere de face a ser atribuído.
 */
void ads_set_face(address_t *a, char face);

/** @brief   Define o número do endereço.
 *
 * @param   a       Ponteiro para o endereço.
 * @param   number  Novo número a ser atribuído.
 */
void ads_set_number(address_t *a, uint16_t number);

/** @brief   Destrói a estrutura de endereço, liberando a memória alocada.
 *
 * @param   ads  Ponteiro para o endereço a ser destruído.
 */
void ads_destroy(address_t *ads);

#endif
