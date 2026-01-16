#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "symbols.h" // Deve vir antes do parser se o parser usa tipos dele
#include "lexer.h"
#include "parser.h"
#include "coord.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <arquivo.txt>\n", argv[0]);
        return 1;
    }

    // 1. Carregamento do arquivo para a memória
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

    // 2. Inicialização do ambiente
    char *ptr = code;
    symbol_table_t symbol_table = { .count = 0 };
    coordinate_t current = {0, 0, 0};

    while (*ptr != '\0') {
        // Pula espaços e linhas vazias
        while (*ptr && isspace(*ptr)) ptr++;
        if (*ptr == '\0') break;

        char *ptr_antes = ptr;
        token_t t = get_next_token(&ptr);

        // 3. Verifica se o próximo token é um '=' (Atribuição)
        char *ptr_temp = ptr;
        token_t next = get_next_token(&ptr_temp);

        if (next.type == TOKEN_SYMBOL && next.text[0] == '=') {
            char var_name[32];
            strncpy(var_name, t.text, 31);
            ptr = ptr_temp; // Avança o ponteiro para depois do '='

            // Espia o que vem após o '=' para decidir se é número ou comando
            char *ptr_valor = ptr;
            token_t v_tok = get_next_token(&ptr_valor);

            if (v_tok.type == TOKEN_NUMBER) {
                // Caso: fator = 5
                coordinate_t c = {v_tok.value, 0, 0}; 
                set_variable(&symbol_table, var_name, c);
                ptr = ptr_valor; 
                printf("Variavel escalar '%s' definida como %d\n", var_name, v_tok.value);
            } 
            else {
                // Caso: p1 = CREATE(...)
                ptr = ptr_temp; // Reset para o parser ler o comando
                command_node_t cmd = parse_line(&ptr, &symbol_table);
                if (strcmp(cmd.command, "CREATE") == 0) {
                    // Resolvemos os argumentos do CREATE
                    int vals[3] = {0,0,0};
                    for(int i=0; i<3; i++) {
                        if(cmd.args[i].type == ARG_NUMBER) vals[i] = cmd.args[i].value;
                        else vals[i] = get_variable(&symbol_table, cmd.args[i].var_name).x;
                    }
                    current = new_coord(vals[0], vals[1], vals[2]);
                    set_variable(&symbol_table, var_name, current);
                    printf("Variavel '%s' criada em (%d, %d, %d)\n", var_name, current.x, current.y, current.z);
                }
            }
        } 
        else {
            // 4. Caso: Comando Direto (sem atribuição)
            ptr = ptr_antes; 
            command_node_t cmd = parse_line(&ptr, &symbol_table);

            if (strcmp(cmd.command, "CREATE") == 0) {
                int x = (cmd.args[0].type == ARG_NUMBER) ? cmd.args[0].value : get_variable(&symbol_table, cmd.args[0].var_name).x;
                int y = (cmd.args[1].type == ARG_NUMBER) ? cmd.args[1].value : get_variable(&symbol_table, cmd.args[1].var_name).y;
                int z = (cmd.args[2].type == ARG_NUMBER) ? cmd.args[2].value : get_variable(&symbol_table, cmd.args[2].var_name).z;
                current = new_coord(x, y, z);
            } 
            else if (strcmp(cmd.command, "SCALE") == 0) {
                int fator = (cmd.args[0].type == ARG_NUMBER) ? cmd.args[0].value : get_variable(&symbol_table, cmd.args[0].var_name).x;
                current = scale_coordinate(current, fator);
                printf("Escalado por %d: Novo estado (%d, %d, %d)\n", fator, current.x, current.y, current.z);
            } 
            else if (strcmp(cmd.command, "PRINT") == 0) {
                coordinate_t target = current;
                if (cmd.arg_count > 0 && cmd.args[0].type == ARG_VARIABLE) {
                    target = get_variable(&symbol_table, cmd.args[0].var_name);
                }
                printf("RESULTADO: x=%d, y=%d, z=%d\n", target.x, target.y, target.z);
            }
        }
    }

    free(code);
    return 0;
}