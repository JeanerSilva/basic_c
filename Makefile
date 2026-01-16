# Nome do executável final
TARGET = bin/programa

# Compilador e Flags
CC = gcc
CFLAGS = -g -Wall -Iinclude

# Diretórios
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# --- AUTOMATIZAÇÃO ---
# Lista todos os arquivos .c na pasta src/
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Gera a lista de arquivos .o na pasta obj/ a partir da lista de .c
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Regra principal
all: $(BIN_DIR) $(OBJ_DIR) $(TARGET)
	./bin/programa parser.txt
# Criação das pastas de build
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

# Linkagem: Une todos os .o encontrados no executável
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) -no-pie

# Regra Genérica: Como transformar qualquer .c em .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza completa
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean

	