#ifndef STREET_H
#define STREET_H

/*
 *  RUA (STREET)
 *
 * O módulo de rua (street) define a estrutura street_t, responsável por representar os
 * atributos de um segmento de rua do sistema viário: o nome da rua, o comprimento e a
 * velocidade média do segmento, e as chaves de CEP das quadras lindeiras a cada lado do
 * segmento (lado direito e lado esquerdo, conforme o sentido do arco no grafo).
 *
 * Um segmento de rua pode não possuir quadra em um dos lados (ou em ambos); esta ausência
 * é indicada, no arquivo de origem, por um hífen (-) e, na estrutura, pela ausência do
 * respectivo CEP (ver street_get_ldir() e street_get_lesq()).
 *
 * Esta estrutura trabalha apenas com as chaves de CEP das quadras lindeiras, nunca com
 * ponteiros diretos para elas, de forma a manter o desacoplamento entre o grafo viário e
 * o armazenamento das quadras.
 *
 * A estrutura pode ser criada por meio de street_init(), clonada com street_clone() e
 * destruída com street_destroy(). São disponibilizadas operações de leitura (get) para
 * todos os campos.
 */

typedef struct street_t street_t;

/** @brief   Inicializa uma nova estrutura de rua com os dados fornecidos.
 *
 * @param   ldir  CEP da quadra do lado direito do segmento de rua, ou "-" caso o segmento
 *                não possua quadra deste lado.
 * @param   lesq  CEP da quadra do lado esquerdo do segmento de rua, ou "-" caso o segmento
 *                não possua quadra deste lado.
 * @param   cmp   Comprimento do segmento de rua, em metros.
 * @param   vm    Velocidade média que os carros trafegam no segmento, em m/s.
 * @param   nome  Nome da rua a qual pertence o segmento.
 *
 * @return  Ponteiro para a nova estrutura street_t inicializada.
 */
street_t *street_init(const char *ldir, const char *lesq, double cmp, double vm, const char *nome);

/** @brief   Clona uma estrutura de rua, criando uma cópia independente.
 *
 * @param   street  Ponteiro para a rua a ser clonada.
 *
 * @return  Ponteiro para uma nova estrutura street_t com os mesmos atributos da original.
 */
street_t *street_clone(const street_t *street);

/** @brief   Recupera o CEP da quadra do lado direito do segmento de rua.
 *
 * @param   street  Ponteiro para a rua.
 *
 * @return  Ponteiro para a string contendo o CEP, ou @c NULL caso o segmento não possua
 *          quadra do lado direito.
 */
const char *street_get_ldir(const street_t *street);

/** @brief   Recupera o CEP da quadra do lado esquerdo do segmento de rua.
 *
 * @param   street  Ponteiro para a rua.
 *
 * @return  Ponteiro para a string contendo o CEP, ou @c NULL caso o segmento não possua
 *          quadra do lado esquerdo.
 */
const char *street_get_lesq(const street_t *street);

/** @brief   Recupera o comprimento do segmento de rua.
 *
 * @param   street  Ponteiro para a rua.
 *
 * @return  Comprimento do segmento, em metros.
 */
double street_get_cmp(const street_t *street);

/** @brief   Recupera a velocidade média de tráfego no segmento de rua.
 *
 * @param   street  Ponteiro para a rua.
 *
 * @return  Velocidade média, em m/s.
 */
double street_get_vm(const street_t *street);

/** @brief   Recupera o nome da rua a qual pertence o segmento.
 *
 * @param   street  Ponteiro para a rua.
 *
 * @return  Ponteiro para a string contendo o nome da rua.
 */
const char *street_get_nome(const street_t *street);

/** @brief   Destrói a estrutura de rua, liberando a memória alocada.
 *
 * @param   street  Ponteiro para a rua a ser destruída.
 */
void street_destroy(street_t *street);

void street_set_vm(street_t *street, double vm);

#endif
