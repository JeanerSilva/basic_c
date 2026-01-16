#pragma once
#include "coord.h"

#define STACK_SIZE 100

typedef struct {
    int ints[STACK_SIZE];
    int int_top;
    
    coordinate_t coords[STACK_SIZE];
    int coord_top;
} stack_t;

void push_int(stack_t *s, int val);
int pop_int(stack_t *s);
void push_coord(stack_t *s, coordinate_t c);
coordinate_t pop_coord(stack_t *s);