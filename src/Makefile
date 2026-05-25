CC = gcc
CFLAGS = -std=c99 -DUNITY_INCLUDE_DOUBLE -Wall -Wextra -Werror -I../include -I../lib
LDFLAGS = -lm

SRC_DIR := .
BUILD_DIR := ../build
BIN := ted

# Arquivos fonte do projeto principal (excluindo main.c para os testes)
SRC_FILES := $(shell find $(SRC_DIR) -name '*.c' ! -name 'main.c')
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

# Arquivos de teste
TEST_DIR := ../tests
TEST_SRCS := $(wildcard $(TEST_DIR)/*.c)
TEST_BINS := $(patsubst $(TEST_DIR)/%.c, ./%, $(TEST_SRCS))

# Arquivos Unity
UNITY_SRCS := ../lib/unity/unity.c
UNITY_OBJS := $(BUILD_DIR)/unity/unity.o

.PHONY: all clean tests run_tests

all: $(BIN)

$(BIN): $(OBJ_FILES) $(BUILD_DIR)/main.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Compilação do Unity
$(UNITY_OBJS): $(UNITY_SRCS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Compilação de cada arquivo de teste em um binário separado
./%: $(TEST_DIR)/%.c $(OBJ_FILES) $(UNITY_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compila todos os testes
tests: $(TEST_BINS)

# Compila e executa todos os testes
run_tests: tests
	@echo "=== Executando testes ==="
	@for test in $(TEST_BINS); do \
		echo "--- $$test ---"; \
		./$$test; \
	done

clean:
	rm -rf $(BUILD_DIR) $(BIN) $(TEST_BINS)
	rm -f *.hf *.hfc *.hfd
