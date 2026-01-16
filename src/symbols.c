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
    return (coordinate_t){0, 0, 0}; // Retorna zero se não achar
}