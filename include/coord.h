#pragma once

// Declaração antecipada para que o ponteiro de função reconheça o tipo
typedef struct Coordinate coordinate_t;

struct Coordinate {
    int x, y, z;
    // "Método" que retorna uma nova coordenada escalada
    coordinate_t (*scale)(coordinate_t *self, int factor);
};

coordinate_t new_coord(int x, int y, int z);