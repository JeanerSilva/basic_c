#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "coord.h"
#include "lexer.h"
#include "symbols.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <arquivo.txt>\n", argv[0]);
        return 1;
    }

    // --- BLOCO DE LEITURA DO ARQUIVO (O que estava faltando) ---
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char *code = malloc(length + 1); // <--- AQUI A VARIAVEL É DECLARADA
    if (!code) {
        fclose(file);
        return 1;
    }
    
    fread(code, 1, length, file);
    code[length] = '\0';
    fclose(file);
    // ----------------------------------------------------------

    char *ptr = code;
    symbol_table_t symbol_table = { .count = 0 };
    coordinate_t current = {0, 0, 0};

    while (*ptr != '\0') {
        while (*ptr && isspace(*ptr)) ptr++;
        if (*ptr == '\0') break;

        char *ptr_antes = ptr;
        token_t t = get_next_token(&ptr);

        // Espiar se o próximo é '='
        char *ptr_temp = ptr;
        token_t next = get_next_token(&ptr_temp);

        if (next.type == TOKEN_SYMBOL && next.text[0] == '=') {
            char var_name[32];
            strncpy(var_name, t.text, 31);
            ptr = ptr_temp; 
            
            command_node_t cmd = parse_line(&ptr, &symbol_table);
            if (strcmp(cmd.command, "CREATE") == 0) {
                current = new_coord(cmd.args[0], cmd.args[1], cmd.args[2]);
                set_variable(&symbol_table, var_name, current);
                printf("Variavel '%s' definida com (%d, %d, %d)\n", 
                        var_name, current.x, current.y, current.z);
            }
        } else {
            ptr = ptr_antes; 
            command_node_t cmd = parse_line(&ptr, &symbol_table);

            if (strcmp(cmd.command, "CREATE") == 0) {
                current = new_coord(cmd.args[0], cmd.args[1], cmd.args[2]);
            } else if (strcmp(cmd.command, "SCALE") == 0) {
                current = scale_coordinate(current, cmd.args[0]);
            } else if (strcmp(cmd.command, "PRINT") == 0) {
                printf("COORD ATUAL: x=%d, y=%d, z=%d\n", current.x, current.y, current.z);
            }
        }
    }

    free(code);
    return 0;
}