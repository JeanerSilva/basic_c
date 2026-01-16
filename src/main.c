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

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *code = malloc(length + 1);
    fread(code, 1, length, file);
    code[length] = '\0';
    fclose(file);

    char *ptr = code;
    symbol_table_t symbol_table = { .count = 0 };
    coordinate_t current = {0, 0, 0};

    while (*ptr != '\0') {
        // Pula espaços iniciais para não ler tokens errados
        while (*ptr && isspace(*ptr)) ptr++;
        if (*ptr == '\0') break;

        char *ptr_antes = ptr;
        token_t t = get_next_token(&ptr);

        // Espia se o próximo token é '='
        char *ptr_temp = ptr;
        token_t next = get_next_token(&ptr_temp);

        if (next.type == TOKEN_SYMBOL && next.text[0] == '=') {
            // CASO ATRIBUIÇÃO: var = ...
            char var_name[32];
            strncpy(var_name, t.text, 31);
            var_name[31] = '\0';
            ptr = ptr_temp; // Avança ponteiro para depois do '='

            char *ptr_check = ptr;
            token_t v_tok = get_next_token(&ptr_check);

            if (v_tok.type == TOKEN_NUMBER) {
                // Atribuição simples: fator = 5
                coordinate_t scalar = {v_tok.value, 0, 0};
                set_variable(&symbol_table, var_name, scalar);
                ptr = ptr_check;
                printf("Definido escalar: %s = %d\n", var_name, v_tok.value);
            } else {
                // Atribuição de comando: p1 = CREATE(...)
                ptr = ptr_temp; 
                command_node_t cmd = parse_line(&ptr, &symbol_table);
                
                if (strcmp(cmd.command, "CREATE") == 0) {
                    int v[3] = {0, 0, 0};
                    for(int i = 0; i < 3 && i < cmd.arg_count; i++) {
                        v[i] = (cmd.args[i].type == ARG_NUMBER) ? 
                                cmd.args[i].value : get_variable(&symbol_table, cmd.args[i].var_name).x;
                    }
                    current = new_coord(v[0], v[1], v[2]);
                    set_variable(&symbol_table, var_name, current);
                    printf("Variavel '%s' criada: (%d, %d, %d)\n", var_name, current.x, current.y, current.z);
                }
                else if (strcmp(cmd.command, "ADD") == 0) {
                    coordinate_t c1 = get_variable(&symbol_table, cmd.args[0].var_name);
                    coordinate_t c2 = get_variable(&symbol_table, cmd.args[1].var_name);
                    current = add_coordinates(c1, c2);
                    set_variable(&symbol_table, var_name, current);
                    printf("Variavel '%s' (SOMA): (%d, %d, %d)\n", var_name, current.x, current.y, current.z);
                }
            }
        } else {
            // CASO COMANDO DIRETO: CREATE(...), SCALE(...), PRINT(...)
            ptr = ptr_antes;
            command_node_t cmd = parse_line(&ptr, &symbol_table);

            if (strcmp(cmd.command, "CREATE") == 0) {
                if (cmd.arg_count == 1 && cmd.args[0].type == ARG_VARIABLE) {
                    current = get_variable(&symbol_table, cmd.args[0].var_name);
                } else {
                    int x = (cmd.args[0].type == ARG_NUMBER) ? cmd.args[0].value : get_variable(&symbol_table, cmd.args[0].var_name).x;
                    int y = (cmd.args[1].type == ARG_NUMBER) ? cmd.args[1].value : get_variable(&symbol_table, cmd.args[1].var_name).x;
                    int z = (cmd.args[2].type == ARG_NUMBER) ? cmd.args[2].value : get_variable(&symbol_table, cmd.args[2].var_name).x;
                    current = new_coord(x, y, z);
                }
            } 
            else if (strcmp(cmd.command, "SCALE") == 0) {
                int f = (cmd.args[0].type == ARG_NUMBER) ? 
                         cmd.args[0].value : get_variable(&symbol_table, cmd.args[0].var_name).x;
                current = scale_coordinate(current, f);
                printf("Escalado por %d: (%d, %d, %d)\n", f, current.x, current.y, current.z);
            } 
            else if (strcmp(cmd.command, "PRINT") == 0) {
                coordinate_t p = (cmd.arg_count > 0 && cmd.args[0].type == ARG_VARIABLE) ? 
                                 get_variable(&symbol_table, cmd.args[0].var_name) : current;
                printf("RESULTADO: x=%d, y=%d, z=%d\n", p.x, p.y, p.z);
            }
        }
    }

    dump_symbol_table(&symbol_table);
    free(code);
    return 0;
}