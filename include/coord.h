#pragma once

typedef struct Coordinate {
    int x;
    int y;
    int z;
} coordinate_t;

coordinate_t new_coord(int x, int y, int z); 
coordinate_t scale_coordinate(coordinate_t coord, int factor); 
coordinate_t add_coordinates(coordinate_t a, coordinate_t b); // Adicione esta linha!