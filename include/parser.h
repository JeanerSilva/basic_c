#pragma once
#include "lexer.h"
#include "symbols.h" // Adicionado

typedef struct {
    char command[32];
    int args[3];
    int arg_count;
} command_node_t;

// Agora aceita a tabela de símbolos como argumento
command_node_t parse_line(char **input, symbol_table_t *table);