#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "coord.h"
#include "lexer.h"
#include "stack.h"

int main(int argc, char *argv[]) {
    // Verifica se o utilizador passou o nome do ficheiro script
    if (argc < 2) {
        printf("Uso: %s <arquivo.txt>\n", argv[0]);
        return 1;
    }

    // 1. ABRIR O FICHEIRO SCRIPT
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Erro ao abrir o ficheiro");
        return 1;
    }

    // 2. LER O CONTEÚDO PARA A MEMÓRIA
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

    // 3. INICIALIZAR A PILHA E O INTERPRETADOR
    char *ptr = code;
    stack_t stack;
    stack.int_top = -1;   // Inicializa topo de inteiros vazio
    stack.coord_top = -1; // Inicializa topo de coordenadas vazio
    
    token_t t;

    // 4. LOOP PRINCIPAL (INTERPRETADOR)
    while ((t = get_next_token(&ptr)).type != TOKEN_EOF) {
        
        // Se for número, vai para a pilha de inteiros
        if (t.type == TOKEN_NUMBER) {
            push_int(&stack, t.value);
        } 
        
        // Se for comando, executa a lógica
        else if (t.type == TOKEN_COMMAND) {
            
            if (strcmp(t.text, "CREATE") == 0) {
                // Tira 3 números da pilha para criar uma coordenada
                int z = pop_int(&stack);
                int y = pop_int(&stack);
                int x = pop_int(&stack);
                push_coord(&stack, new_coord(x, y, z));
            } 
            
            else if (strcmp(t.text, "ADD") == 0) {
                // Tira 2 coordenadas, soma-as e mete o resultado de volta
                coordinate_t c2 = pop_coord(&stack);
                coordinate_t c1 = pop_coord(&stack);
                push_coord(&stack, add_coordinates(c1, c2));
            }
            
            else if (strcmp(t.text, "SCALE") == 0) {
                // Tira 1 número (fator) e 1 coordenada para escalar
                int fator = pop_int(&stack);
                coordinate_t c = pop_coord(&stack);
                push_coord(&stack, scale_coordinate(c, fator));
            }
            
            else if (strcmp(t.text, "PRINT") == 0) {
                // Mostra a coordenada do topo sem a remover permanentemente
                coordinate_t res = pop_coord(&stack);
                printf("RESULTADO: x=%d, y=%d, z=%d\n", res.x, res.y, res.z);
                push_coord(&stack, res); 
            }
        }
    }

    // Limpeza final
    free(code);
    return 0;
}