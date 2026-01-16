#include "stack.h"
#include <stdio.h>

void push_int(stack_t *s, int val) {
    if (s->int_top < STACK_SIZE - 1) {
        s->ints[++(s->int_top)] = val;
    }
}

int pop_int(stack_t *s) {
    if (s->int_top >= 0) {
        return s->ints[(s->int_top)--];
    }
    return 0;
}

void push_coord(stack_t *s, coordinate_t c) {
    if (s->coord_top < STACK_SIZE - 1) {
        s->coords[++(s->coord_top)] = c;
    }
}

coordinate_t pop_coord(stack_t *s) {
    if (s->coord_top >= 0) {
        return s->coords[(s->coord_top)--];
    }
    return (coordinate_t){0, 0, 0};
}