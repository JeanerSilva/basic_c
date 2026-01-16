#pragma once
#include "coord.h"

typedef struct {
    char name[32];
    coordinate_t coord;
} symbol_t;

typedef struct {
    symbol_t vars[50];
    int count;
} symbol_table_t;

void set_variable(symbol_table_t *table, const char *name, coordinate_t c);
coordinate_t get_variable(symbol_table_t *table, const char *name);// ... outras declarações ...
void dump_symbol_table(symbol_table_t *table);

#include <stdio.h>
#include <string.h>
#include "symbols.h"

void dump_symbol_table(symbol_table_t *table) {
    printf("\n========== DEBUG: TABELA DE SIMBOLOS ==========\n");
    printf("Total de variaveis: %d\n", table->count);
    for (int i = 0; i < table->count; i++) {
        printf("[%d] ID: '%s' -> Valor: (x:%d, y:%d, z:%d)\n", 
               i, 
               table->vars[i].name[0] == '\0' ? "(VAZIO)" : table->vars[i].name, 
               table->vars[i].coord.x, 
               table->vars[i].coord.y, 
               table->vars[i].coord.z);
    }
    printf("================================================\n\n");
}