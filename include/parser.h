#pragma once
#include "lexer.h"
#include "coord.h"

typedef struct {
    char command[32];
    int args[3];
    int arg_count;
} command_node_t;

// Função que analisa uma linha de comando completa
command_node_t parse_line(char **input);