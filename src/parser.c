#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "lexer.h"
#include "symbols.h"

void report_error(const char* message, int line) {
    fprintf(stderr, "ERRO DE SINTAXE na linha %d: %s\n", line, message);
    exit(1);
}

command_node_t parse_line(char **input, symbol_table_t *table) {
    command_node_t node;
    node.arg_count = 0;
    memset(node.command, 0, sizeof(node.command));

    token_t t = get_next_token(input);
    if (t.type == TOKEN_EOF) return node;
    
    strcpy(node.command, t.text);

    t = get_next_token(input); // Espera '('
    if (t.type != TOKEN_SYMBOL || t.text[0] != '(') {
        report_error("Esperado '('", t.line);
    }

    while (1) {
        t = get_next_token(input);
        if (t.type == TOKEN_SYMBOL && t.text[0] == ')') break;

        if (t.type == TOKEN_NUMBER) {
            node.args[node.arg_count++] = t.value;
        } 
        else if (t.type == TOKEN_COMMAND) {
            // BUSCA NA TABELA: Se for um nome, tenta pegar o valor X da coordenada
            coordinate_t v = get_variable(table, t.text);
            node.args[node.arg_count++] = v.x; // Usando X como exemplo de valor numérico
        }

        t = get_next_token(input);
        if (t.type == TOKEN_SYMBOL && t.text[0] == ')') break;
        if (t.type != TOKEN_SYMBOL || t.text[0] != ',') {
            report_error("Esperado ',' ou ')'", t.line);
        }
    }
    return node;
}