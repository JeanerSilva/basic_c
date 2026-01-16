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
coordinate_t add_coordinates(coordinate_t c1, coordinate_t c2) {
    coordinate_t result;
    result.x = c1.x + c2.x;
    result.y = c1.y + c2.y;
    result.z = c1.z + c2.z;
    return result;
}