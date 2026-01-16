#include "coord.h"

coordinate_t new_coord (int x, int y, int z) { 
    coordinate_t c = { .x = x, .y = y, .z = z }; 
    return c; 
};

coordinate_t scale_coordinate(coordinate_t coord, int factor) { 
    coordinate_t scaled = {
        .x = coord.x * factor, 
        .y = coord.y * factor, 
        .z = coord.z * factor  
    };
    return scaled; 
};

// Implementação da nova função
coordinate_t add_coordinates(coordinate_t a, coordinate_t b) {
    coordinate_t result = {
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z
    };
    return result;
}