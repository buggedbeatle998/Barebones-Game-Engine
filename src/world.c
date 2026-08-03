#include "world.h"


#include "engine/sprites.h"
#include "lib/text.h"



static void handle_move(Sprites *sprites, Keys *keys);


static const Font *pxl_font = &(Font){0, 0, 0, 5, 7, 1, 1, 2, 47};
static int jump = 0;
static int height = 0;


int world_init(Sprites *sprites) {
    sprite_append(sprites, 1, 0, 0, 32, 2, 40, 40, 0, 0);
    blend_set(sprites, 0, .75f, 0.f, 0.75f, 1.f);
    sprite_append(sprites, 1, 32, 0, 32, 32, 10, 10, 640, 80);
    sprite_append(sprites, 1, 32, 0, 32, 32, -10, 10, 640, 80);
    update_num(sprites);
    update_range(sprites, 0, 3, true, true, true, true);

    return 0;
}


int world_step(Sprites *sprites, Keys *keys) {
    handle_move(sprites, keys);

    return 0;
}


void world_destroy(void) {
    
}


#define SPEED 10
#define MIN_JUMP 30
void handle_move(Sprites *sprites, Keys *keys) {
    if (!height && key_get_down(keys, SDLK_Z))
        jump = 32;
    else if (jump > 5 && !key_get_pressed(keys, SDLK_Z)) {
        jump = 5;
    }
    int chy = jump > MIN_JUMP ? MIN_JUMP : jump;
    if (-chy < height) {
        jump -= 2;
        height += chy;
        matrix_translate(sprites, 1, 0, chy);
    } else {
        matrix_translate(sprites, 1, 0, -height);
        height = 0;
    }

    int chx = key_get_pressed(keys, SDLK_RIGHT) - key_get_pressed(keys, SDLK_LEFT);
    if (chx) {
        matrix_translate(sprites, 1, chx * SPEED, 0);
    }
    if (chx || jump)
        update_range(sprites, 1, 1, true, false, false, false);

    if (!height)
        jump = 0;
}
