#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "lexer.h"
#include "symbols.h"

// Função interna para reportar erros de sintaxe e abortar
void report_error(const char* message, int line) {
    fprintf(stderr, "ERRO DE SINTAXE na linha %d: %s\n", line, message);
    exit(1);
}

command_node_t parse_line(char **input, symbol_table_t *table) {
    command_node_t node;
    node.arg_count = 0;
    memset(node.command, 0, sizeof(node.command));

    // 1. Identifica o comando (ex: CREATE, SCALE, PRINT)
    token_t t = get_next_token(input);
    if (t.type == TOKEN_EOF) return node;
    
    if (t.type != TOKEN_COMMAND) {
        report_error("Esperado um nome de comando ou variavel.", t.line);
    }
    strncpy(node.command, t.text, sizeof(node.command) - 1);

    // 2. Espera o símbolo '('
    t = get_next_token(input);
    if (t.type != TOKEN_SYMBOL || t.text[0] != '(') {
        report_error("Esperado '(' apos o comando.", t.line);
    }

    // 3. Loop para ler os argumentos
    while (1) {
        t = get_next_token(input);
        
        // Verifica se chegamos ao fim dos argumentos sem ler nada PRINT()
        if (t.type == TOKEN_SYMBOL && t.text[0] == ')') {
            break;
        }

        // Analisa se o argumento é um NÚMERO ou uma VARIÁVEL
        if (t.type == TOKEN_NUMBER) {
            node.args[node.arg_count].type = ARG_NUMBER;
            node.args[node.arg_count].value = t.value;
            node.arg_count++;
        } 
        else if (t.type == TOKEN_COMMAND) {
            // Se o Lexer leu como texto, tratamos como referência a variável
            node.args[node.arg_count].type = ARG_VARIABLE;
            strncpy(node.args[node.arg_count].var_name, t.text, 31);
            node.arg_count++;
        }
        else {
            report_error("Argumento invalido. Esperado numero ou variavel.", t.line);
        }

        // Verifica o próximo token: pode ser ',' (continua) ou ')' (encerra)
        t = get_next_token(input);
        if (t.type == TOKEN_SYMBOL && t.text[0] == ')') {
            break;
        }
        
        if (t.type != TOKEN_SYMBOL || t.text[0] != ',') {
            report_error("Esperado ',' ou ')' entre argumentos.", t.line);
        }

        // Limite de segurança para não estourar o array de argumentos
        if (node.arg_count >= 3) {
            // Consome o ')' final se o usuário passou 3 argumentos
            t = get_next_token(input);
            if (t.type != TOKEN_SYMBOL || t.text[0] != ')') {
                report_error("Excesso de argumentos ou falta de ')'.", t.line);
            }
            break;
        }
    }
    
    return node;
}