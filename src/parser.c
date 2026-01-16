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
        
        // Se encontrar ')', o comando acabou (ex: PRINT())
        if (t.type == TOKEN_SYMBOL && t.text[0] == ')') {
            break;
        }

        if (t.type == TOKEN_NUMBER) {
            node.args[node.arg_count].type = ARG_NUMBER;
            node.args[node.arg_count].value = t.value;
            node.arg_count++;
        } 
        else if (t.type == TOKEN_COMMAND) {
            // Aqui p1, p2, fator_escala, etc., são capturados
            node.args[node.arg_count].type = ARG_VARIABLE;
            strncpy(node.args[node.arg_count].var_name, t.text, 31);
            node.arg_count++;
        }
        else {
            char msg[128]; // Aumentado de 64 para 128
            snprintf(msg, sizeof(msg), "Argumento invalido '%s'. Esperado numero ou variavel.", t.text);
            report_error(msg, t.line);
        }

        // Após ler um argumento, precisamos verificar o que vem depois
        t = get_next_token(input);
        
        if (t.type == TOKEN_SYMBOL && t.text[0] == ')') {
            break; // Fim correto dos argumentos
        }
        
        if (t.type != TOKEN_SYMBOL || t.text[0] != ',') {
            // Erro se não houver vírgula separando os argumentos
            report_error("Esperado ',' ou ')' entre os argumentos.", t.line);
        }
    }
    
    return node;
}