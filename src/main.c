#include <stdio.h>    
#include "coord.h"    

int main() {
    coordinate_t c = new_coord(1, 2, 3);
    
    // Chamamos o "método" através do ponteiro de função
    // Precisamos passar &c porque em C o 'self/this' não é automático
    coordinate_t c_scaled = c.scale(&c, 2);

    printf("x: %d, y: %d, z: %d\n", c_scaled.x, c_scaled.y, c_scaled.z);
    
    return 0;
}