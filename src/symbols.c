#include "symbols.h"
#include <string.h>
#include <stdio.h>

void set_variable(symbol_table_t *table, const char *name, coordinate_t c) {
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->vars[i].name, name) == 0) {
            table->vars[i].coord = c;
            return;
        }
    }
    strcpy(table->vars[table->count].name, name);
    table->vars[table->count].coord = c;
    table->count++;
}

coordinate_t get_variable(symbol_table_t *table, const char *name) {
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->vars[i].name, name) == 0) {
            return table->vars[i].coord;
        }
    }
    // Se não achar, retorna zerado (ou você pode imprimir um erro aqui)
    coordinate_t empty = {0, 0, 0};
    return empty;
}

void dump_symbol_table(symbol_table_t *table) {
    printf("\n========== DEBUG: TABELA DE SIMBOLOS ==========\n");
    printf("Total de variaveis: %d\n", table->count);
    for (int i = 0; i < table->count; i++) {
        printf("[%d] ID: '%s' -> Valor: (x:%d, y:%d, z:%d)\n", 
               i, table->vars[i].name, 
               table->vars[i].coord.x, table->vars[i].coord.y, table->vars[i].coord.z);
    }
    printf("================================================\n\n");
}