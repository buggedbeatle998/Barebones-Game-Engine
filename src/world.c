#include "world.h"

#include "sprites.h"


int world_init(Sprites *sprites) {
    sprite_append(sprites, 1, 0, 0, 32, 16, 30, 30, 30, 100);
    blend_set(sprites, 0, 1.f, 0.f, 0.5f, 1.f);
    sprite_append(sprites, 0, 0, 0, 32, 32, 30, 15, 0, 0);
    blend_set(sprites, 1, 1.f, 1.f, 1.f, 0.75f);
    update_num(sprites);
    update_range(sprites, 0, 2, true, true, true, true);

    return 0;
}


int world_step(Sprites *sprites) {
    matrix_translate(sprites, 1, 1, 0);
    update_range(sprites, 1, 1, true, false, false, false);

    return 0;
}


void world_destroy(void) {
    
}
