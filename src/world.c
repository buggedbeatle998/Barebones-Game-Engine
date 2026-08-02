#include "world.h"


#include "engine/sprites.h"
#include "lib/text.h"

#define SPEED 5


static void handle_move(Sprites *sprites, Keys *keys);


static const Font *pxl_font = &(Font){0, 0, 0, 5, 7, 1, 1, 2, 47};


int world_init(Sprites *sprites) {
    sprite_append(sprites, 1, 0, 0, 32, 16, 30, 30, 30, 100);
    blend_set(sprites, 0, 1.f, 0.f, 0.5f, 1.f);
    sprite_append(sprites, 1, 32, 0, 32, 32, 30, 15, 0, 0);
    blend_set(sprites, 1, 1.f, 1.f, 1.f, 0.75f);
    update_num(sprites);
    update_range(sprites, 0, 2, true, true, true, true);
    
    char text[] = "HELLO\nWORLD";
    draw_text(sprites, pxl_font, sizeof(text) - 1, text, 0, 1, 1, 10, 100, 10, 10);

    return 0;
}


int world_step(Sprites *sprites, Keys *keys) {
    handle_move(sprites, keys);

    //matrix_translate(sprites, 1, 1, 0);
    //update_range(sprites, 1, 1, true, false, false, false);

    return 0;
}


void world_destroy(void) {
    
}


void handle_move(Sprites *sprites, Keys *keys) {
    int chx = key_get_pressed(keys, SDLK_RIGHT) - key_get_pressed(keys, SDLK_LEFT);
    int chy = key_get_pressed(keys, SDLK_UP) - key_get_pressed(keys, SDLK_DOWN);
    if (chx || chy) {
        matrix_translate(sprites, 1, chx * SPEED, chy * SPEED);
        update_range(sprites, 1, 1, true, false, false, false);
    }
}
