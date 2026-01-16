#include <string.h>
#include <stdio.h>
#include "parser.h"

command_node_t parse_line(char **input) {
    command_node_t node;
    node.arg_count = 0;

    // 1. Pega o nome do comando (ex: CREATE)
    token_t t = get_next_token(input);
    strcpy(node.command, t.text);

    // 2. Espera '(' (Vamos simplificar o lexer para tratar isso)
    while (**input == ' ' || **input == '(') (*input)++;

    // 3. Lê os argumentos até achar ')'
    while (**input != ')' && **input != '\0') {
        token_t arg = get_next_token(input);
        if (arg.type == TOKEN_NUMBER) {
            node.args[node.arg_count++] = arg.value;
        }
        if (**input == ',') (*input)++; // Pula vírgulas
    }
    
    if (**input == ')') (*input)++; // Consome o fecha parênteses
    return node;
}