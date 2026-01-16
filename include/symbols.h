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
coordinate_t get_variable(symbol_table_t *table, const char *name);