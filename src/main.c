#include <stdio.h>    // Necessário para o printf 
#include "coord.h"    // Necessário para o tipo coordinate_t e new_coord [cite: 1, 2]

int main() {
    // 1. Criamos a "instância"
    coordinate_t c = new_coord(1, 2, 3);
    
    // 2. Chamamos o "método" através do ponteiro de função
    // Precisamos passar &c porque em C o 'self/this' não é automático
    coordinate_t c_scaled = c.scale(&c, 2);

    printf("x: %d, y: %d, z: %d\n", c_scaled.x, c_scaled.y, c_scaled.z);
    
    return 0;
}