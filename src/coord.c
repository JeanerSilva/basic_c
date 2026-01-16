#include "coord.h"

// Função interna que será usada como "método"
static coordinate_t scale_impl(coordinate_t *self, int factor) {
    return new_coord(self->x * factor, self->y * factor, self->z * factor);
}

coordinate_t new_coord(int x, int y, int z) {
    coordinate_t c;
    c.x = x;
    c.y = y;
    c.z = z;
    // Atribuindo a função ao "método"
    c.scale = scale_impl;
    return c;
}