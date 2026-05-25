#include "unity/unity.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

void test_rstrcat_concatena_duas_strings_simples(void) {
  char *result = r_strcat("hello", " world");
  TEST_ASSERT_EQUAL_STRING("hello world", result);
  free(result);
}

void test_rstrcat_string_vazia_esquerda(void) {
  char *result = r_strcat("", "suffix");
  TEST_ASSERT_EQUAL_STRING("suffix", result);
  free(result);
}

void test_rstrcat_string_vazia_direita(void) {
  char *result = r_strcat("prefix", "");
  TEST_ASSERT_EQUAL_STRING("prefix", result);
  free(result);
}

void test_rstrcat_ambas_vazias(void) {
  char *result = r_strcat("", "");
  TEST_ASSERT_EQUAL_STRING("", result);
  free(result);
}

void test_rstrcat_retorna_ponteiro_diferente_dos_originais(void) {
  char s1[] = "abc";
  char s2[] = "def";
  char *result = r_strcat(s1, s2);

  TEST_ASSERT_NOT_NULL(result);
  TEST_ASSERT_TRUE(result != s1);
  TEST_ASSERT_TRUE(result != s2);

  free(result);
}

void test_rstrcat_comprimento_correto_do_resultado(void) {
  char *result = r_strcat("abcd", "efgh");

  TEST_ASSERT_EQUAL_INT(8, (int)strlen(result));
  TEST_ASSERT_EQUAL_STRING("abcdefgh", result);

  free(result);
}

void test_rstrcat_strings_longas(void) {
  const char *s1 = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
  const char *s2 = "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB";

  char *result = r_strcat((char *)s1, (char *)s2);

  TEST_ASSERT_EQUAL_INT(128, (int)strlen(result));

  for (int i = 0;  i < 64;  i++) TEST_ASSERT_EQUAL_CHAR('A', result[i]);
  for (int i = 64; i < 128; i++) TEST_ASSERT_EQUAL_CHAR('B', result[i]);

  free(result);
}

void test_rstrcat_preserva_conteudo_das_strings_originais(void) {
  char s1[] = "foo";
  char s2[] = "bar";

  char *result = r_strcat(s1, s2);

  TEST_ASSERT_EQUAL_STRING("foo", s1);
  TEST_ASSERT_EQUAL_STRING("bar", s2);

  free(result);
}

void test_rstrcat_com_extensao_de_arquivo(void) {
  char *result = r_strcat("./saida/dados", ".hf");
  TEST_ASSERT_EQUAL_STRING("./saida/dados.hf", result);
  free(result);
}

void test_strnlen_string_normal_dentro_do_limite(void) {
  size_t len = strnlen("hello", 10);
  TEST_ASSERT_EQUAL_size_t(5, len);
}

void test_strnlen_limite_exatamente_igual_ao_comprimento(void) {
  size_t len = strnlen("abc", 3);
  TEST_ASSERT_EQUAL_size_t(3, len);
}

void test_strnlen_limite_menor_que_comprimento_real(void) {
  size_t len = strnlen("abcdef", 4);
  TEST_ASSERT_EQUAL_size_t(4, len);
}

void test_strnlen_limite_zero_retorna_zero(void) {
  size_t len = strnlen("qualquer", 0);
  TEST_ASSERT_EQUAL_size_t(0, len);
}

void test_strnlen_string_vazia_retorna_zero(void) {
  size_t len = strnlen("", 10);
  TEST_ASSERT_EQUAL_size_t(0, len);
}

void test_strnlen_string_vazia_maxlen_zero(void) {
  size_t len = strnlen("", 0);
  TEST_ASSERT_EQUAL_size_t(0, len);
}

void test_strnlen_um_unico_caractere(void) {
  size_t len = strnlen("x", 5);
  TEST_ASSERT_EQUAL_size_t(1, len);
}

void test_strnlen_maxlen_muito_maior_que_string(void) {
  size_t len = strnlen("hi", 1000);
  TEST_ASSERT_EQUAL_size_t(2, len);
}

void test_strnlen_limite_1_string_de_tamanho_1(void) {
  size_t len = strnlen("a", 1);
  TEST_ASSERT_EQUAL_size_t(1, len);
}

void test_strnlen_limite_1_string_maior(void) {
  size_t len = strnlen("abc", 1);
  TEST_ASSERT_EQUAL_size_t(1, len);
}

void test_strnlen_string_com_espacos(void) {
  size_t len = strnlen("a b c", 10);
  TEST_ASSERT_EQUAL_size_t(5, len);
}

int main(void) {
  UNITY_BEGIN();

  // rstrcat
  RUN_TEST(test_rstrcat_concatena_duas_strings_simples);
  RUN_TEST(test_rstrcat_string_vazia_esquerda);
  RUN_TEST(test_rstrcat_string_vazia_direita);
  RUN_TEST(test_rstrcat_ambas_vazias);
  RUN_TEST(test_rstrcat_retorna_ponteiro_diferente_dos_originais);
  RUN_TEST(test_rstrcat_comprimento_correto_do_resultado);
  RUN_TEST(test_rstrcat_strings_longas);
  RUN_TEST(test_rstrcat_preserva_conteudo_das_strings_originais);
  RUN_TEST(test_rstrcat_com_extensao_de_arquivo);

  // strnlen
  RUN_TEST(test_strnlen_string_normal_dentro_do_limite);
  RUN_TEST(test_strnlen_limite_exatamente_igual_ao_comprimento);
  RUN_TEST(test_strnlen_limite_menor_que_comprimento_real);
  RUN_TEST(test_strnlen_limite_zero_retorna_zero);
  RUN_TEST(test_strnlen_string_vazia_retorna_zero);
  RUN_TEST(test_strnlen_string_vazia_maxlen_zero);
  RUN_TEST(test_strnlen_um_unico_caractere);
  RUN_TEST(test_strnlen_maxlen_muito_maior_que_string);
  RUN_TEST(test_strnlen_limite_1_string_de_tamanho_1);
  RUN_TEST(test_strnlen_limite_1_string_maior);
  RUN_TEST(test_strnlen_string_com_espacos);

  return UNITY_END();
}
