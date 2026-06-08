#include "unity/unity.h"
#include "datast/vector.h"

void setUp(void) {}
void tearDown(void) {}

/* ── vec_init ──────────────────────────────────────────────────────────────── */

void test_vec_init_retorna_nao_nulo(void) {
  vector_t *v = vec_init(sizeof(int));
  TEST_ASSERT_NOT_NULL(v);
  vec_destroy(v);
}

void test_vec_init_comeca_vazio(void) {
  vector_t *v = vec_init(sizeof(int));
  TEST_ASSERT_TRUE(vec_is_empty(v));
  TEST_ASSERT_EQUAL_size_t(0, vec_get_size(v));
  vec_destroy(v);
}

/* ── vec_push_back / vec_get_size / vec_is_empty ───────────────────────────── */

void test_push_back_incrementa_tamanho(void) {
  vector_t *v = vec_init(sizeof(int));
  int x = 42;
  vec_push_back(v, &x);
  TEST_ASSERT_EQUAL_size_t(1, vec_get_size(v));
  TEST_ASSERT_FALSE(vec_is_empty(v));
  vec_destroy(v);
}

void test_push_back_multiplos_elementos(void) {
  vector_t *v = vec_init(sizeof(int));
  for (int i = 0; i < 100; i++)
    vec_push_back(v, &i);
  TEST_ASSERT_EQUAL_size_t(100, vec_get_size(v));
  vec_destroy(v);
}

/* ── vec_at ────────────────────────────────────────────────────────────────── */

void test_at_retorna_valores_corretos(void) {
  vector_t *v = vec_init(sizeof(int));
  int valores[] = {10, 20, 30};
  for (int i = 0; i < 3; i++)
    vec_push_back(v, &valores[i]);

  TEST_ASSERT_EQUAL_INT(10, *(int *)vec_at(v, 0));
  TEST_ASSERT_EQUAL_INT(20, *(int *)vec_at(v, 1));
  TEST_ASSERT_EQUAL_INT(30, *(int *)vec_at(v, 2));
  vec_destroy(v);
}

void test_at_indice_fora_dos_limites_retorna_null(void) {
  vector_t *v = vec_init(sizeof(int));
  int x = 1;
  vec_push_back(v, &x);
  TEST_ASSERT_NULL(vec_at(v, 1));
  TEST_ASSERT_NULL(vec_at(v, 999));
  vec_destroy(v);
}

void test_at_vetor_vazio_retorna_null(void) {
  vector_t *v = vec_init(sizeof(int));
  TEST_ASSERT_NULL(vec_at(v, 0));
  vec_destroy(v);
}

/* ── vec_front / vec_back ──────────────────────────────────────────────────── */

void test_front_retorna_primeiro_elemento(void) {
  vector_t *v = vec_init(sizeof(int));
  int valores[] = {1, 2, 3};
  for (int i = 0; i < 3; i++)
    vec_push_back(v, &valores[i]);
  TEST_ASSERT_EQUAL_INT(1, *(int *)vec_front(v));
  vec_destroy(v);
}

void test_back_retorna_ultimo_elemento(void) {
  vector_t *v = vec_init(sizeof(int));
  int valores[] = {1, 2, 3};
  for (int i = 0; i < 3; i++)
    vec_push_back(v, &valores[i]);
  TEST_ASSERT_EQUAL_INT(3, *(int *)vec_back(v));
  vec_destroy(v);
}

void test_front_vetor_vazio_retorna_null(void) {
  vector_t *v = vec_init(sizeof(int));
  TEST_ASSERT_NULL(vec_front(v));
  vec_destroy(v);
}

void test_back_vetor_vazio_retorna_null(void) {
  vector_t *v = vec_init(sizeof(int));
  TEST_ASSERT_NULL(vec_back(v));
  vec_destroy(v);
}

void test_front_e_back_com_um_elemento(void) {
  vector_t *v = vec_init(sizeof(int));
  int x = 99;
  vec_push_back(v, &x);
  TEST_ASSERT_EQUAL_INT(99, *(int *)vec_front(v));
  TEST_ASSERT_EQUAL_INT(99, *(int *)vec_back(v));
  vec_destroy(v);
}

/* ── vec_pop_back ──────────────────────────────────────────────────────────── */

void test_pop_back_decrementa_tamanho(void) {
  vector_t *v = vec_init(sizeof(int));
  int x = 7;
  vec_push_back(v, &x);
  vec_pop_back(v);
  TEST_ASSERT_EQUAL_size_t(0, vec_get_size(v));
  TEST_ASSERT_TRUE(vec_is_empty(v));
  vec_destroy(v);
}

void test_pop_back_vetor_vazio_nao_falha(void) {
  vector_t *v = vec_init(sizeof(int));
  vec_pop_back(v);
  TEST_ASSERT_EQUAL_size_t(0, vec_get_size(v));
  vec_destroy(v);
}

void test_pop_back_elemento_correto_removido(void) {
  vector_t *v = vec_init(sizeof(int));
  int valores[] = {1, 2, 3};
  for (int i = 0; i < 3; i++)
    vec_push_back(v, &valores[i]);
  vec_pop_back(v);
  TEST_ASSERT_EQUAL_size_t(2, vec_get_size(v));
  TEST_ASSERT_EQUAL_INT(2, *(int *)vec_back(v));
  vec_destroy(v);
}

/* ── vec_insert_at ─────────────────────────────────────────────────────────── */

void test_insert_at_inicio(void) {
  vector_t *v = vec_init(sizeof(int));
  int a = 10, b = 20;
  vec_push_back(v, &b);
  vec_insert_at(v, &a, 0);
  TEST_ASSERT_EQUAL_size_t(1, vec_get_size(v));
  TEST_ASSERT_EQUAL_INT(10, *(int *)vec_at(v, 0));
  vec_destroy(v);
}

void test_insert_at_meio(void) {
  vector_t *v = vec_init(sizeof(int));
  int valores[] = {1, 2, 3};
  for (int i = 0; i < 3; i++)
    vec_push_back(v, &valores[i]);
  int novo = 99;
  vec_insert_at(v, &novo, 1);
  TEST_ASSERT_EQUAL_size_t(3, vec_get_size(v));
  TEST_ASSERT_EQUAL_INT(1,  *(int *)vec_at(v, 0));
  TEST_ASSERT_EQUAL_INT(99, *(int *)vec_at(v, 1));
  TEST_ASSERT_EQUAL_INT(3,  *(int *)vec_at(v, 2));
  vec_destroy(v);
}
void test_insert_at_index_igual_tamanho_equivale_push_back(void) {
  vector_t *v = vec_init(sizeof(int));
  int x = 5, y = 10;
  vec_push_back(v, &x);
  bool ok = vec_insert_at(v, &y, 1);
  TEST_ASSERT_TRUE(ok);
  TEST_ASSERT_EQUAL_INT(10, *(int *)vec_back(v));
  TEST_ASSERT_EQUAL_size_t(2, vec_get_size(v));
  vec_destroy(v);
}

void test_insert_at_index_invalido_retorna_false(void) {
  vector_t *v = vec_init(sizeof(int));
  int x = 1;
  bool ok = vec_insert_at(v, &x, 5);
  TEST_ASSERT_FALSE(ok);
  TEST_ASSERT_EQUAL_size_t(0, vec_get_size(v));
  vec_destroy(v);
}

/* ── elementos de tamanho maior (double) ───────────────────────────────────── */

void test_push_back_e_at_com_double(void) {
  vector_t *v = vec_init(sizeof(double));
  double valores[] = {1.1, 2.2, 3.3};
  for (int i = 0; i < 3; i++)
    vec_push_back(v, &valores[i]);
  TEST_ASSERT_DOUBLE_WITHIN(0.001, 1.1, *(double *)vec_at(v, 0));
  TEST_ASSERT_DOUBLE_WITHIN(0.001, 2.2, *(double *)vec_at(v, 1));
  TEST_ASSERT_DOUBLE_WITHIN(0.001, 3.3, *(double *)vec_at(v, 2));
  vec_destroy(v);
}

/* ── push_back preserva cópia independente do dado original ────────────────── */

void test_push_back_copia_valor_independente(void) {
  vector_t *v = vec_init(sizeof(int));
  int x = 42;
  vec_push_back(v, &x);
  x = 99;
  TEST_ASSERT_EQUAL_INT(42, *(int *)vec_at(v, 0));
  vec_destroy(v);
}

/* ── main ──────────────────────────────────────────────────────────────────── */

int main(void) {
  UNITY_BEGIN();

  // vec_init
  RUN_TEST(test_vec_init_retorna_nao_nulo);
  RUN_TEST(test_vec_init_comeca_vazio);

  // vec_push_back / vec_get_size / vec_is_empty
  RUN_TEST(test_push_back_incrementa_tamanho);
  RUN_TEST(test_push_back_multiplos_elementos);

  // vec_at
  RUN_TEST(test_at_retorna_valores_corretos);
  RUN_TEST(test_at_indice_fora_dos_limites_retorna_null);
  RUN_TEST(test_at_vetor_vazio_retorna_null);

  // vec_front / vec_back
  RUN_TEST(test_front_retorna_primeiro_elemento);
  RUN_TEST(test_back_retorna_ultimo_elemento);
  RUN_TEST(test_front_vetor_vazio_retorna_null);
  RUN_TEST(test_back_vetor_vazio_retorna_null);
  RUN_TEST(test_front_e_back_com_um_elemento);

  // vec_pop_back
  RUN_TEST(test_pop_back_decrementa_tamanho);
  RUN_TEST(test_pop_back_vetor_vazio_nao_falha);
  RUN_TEST(test_pop_back_elemento_correto_removido);

  // vec_insert_at
  RUN_TEST(test_insert_at_inicio);
  RUN_TEST(test_insert_at_meio);
  RUN_TEST(test_insert_at_index_igual_tamanho_equivale_push_back);
  RUN_TEST(test_insert_at_index_invalido_retorna_false);

  // compatibilidade com outros tipos
  RUN_TEST(test_push_back_e_at_com_double);

  // semântica de cópia
  RUN_TEST(test_push_back_copia_valor_independente);

  return UNITY_END();
}
