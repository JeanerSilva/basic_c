#pragma once
#include "lexer.h"
#include "symbols.h"

// Definição dos tipos de argumentos
typedef enum { ARG_NUMBER, ARG_VARIABLE } arg_type_t;

typedef struct {
    arg_type_t type;
    int value;
    char var_name[32];
} argument_t;

// Estrutura do comando
typedef struct {
    char command[32];
    argument_t args[3];
    int arg_count;
} command_node_t;

// O protótipo DEVE incluir a symbol_table_t
command_node_t parse_line(char **input, symbol_table_t *table);