#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "coord.h"
#include "lexer.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <arquivo.txt>\n", argv[0]);
        return 1;
    }

    // 1. Abrir o arquivo
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // 2. Ler o conteúdo do arquivo para a variável 'code'
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *code = malloc(length + 1);
    if (!code) {
        fclose(file);
        return 1;
    }
    fread(code, 1, length, file);
    code[length] = '\0';
    fclose(file);

    // 3. Processar o código usando o Parser
    char *ptr = code;
    coordinate_t current = {0, 0, 0};

    while (*ptr != '\0') {
        // Pular espaços em branco ou quebras de linha entre comandos
        while (*ptr == ' ' || *ptr == '\n' || *ptr == '\r' || *ptr == '\t') ptr++;
        if (*ptr == '\0') break;

        command_node_t cmd = parse_line(&ptr);

        if (strcmp(cmd.command, "CREATE") == 0) {
            current = new_coord(cmd.args[0], cmd.args[1], cmd.args[2]);
            printf("Criado: (%d, %d, %d)\n", current.x, current.y, current.z);
        } 
        else if (strcmp(cmd.command, "SCALE") == 0) {
            current = scale_coordinate(current, cmd.args[0]);
            printf("Escalado para: (%d, %d, %d)\n", current.x, current.y, current.z);
        }
        else if (strcmp(cmd.command, "PRINT") == 0) {
            printf("Coordenada Atual: x=%d, y=%d, z=%d\n", current.x, current.y, current.z);
        }
    }

    free(code);
    return 0;
}