#ifndef STRUCTS_PUBLIC
#define STRUCTS_PUBLIC


#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "SDL3/SDL_keycode.h"

#define KEY_STRINGS 3
#define SCANCODE_STRINGS 5
#define EXTENDED_STRINGS 1


typedef struct {
    size_t num_sprites;
    float *mat_pos;
    int32_t *tex_i;
    uint32_t *tex_quad;
    float *blend;
    uint buff_indir;
    uint buff_mats;
    uint buff_texis;
    uint buff_quads;
    uint buff_blends;
} Sprites;


typedef struct {
    uint64_t key_pressed[KEY_STRINGS];
    uint64_t key_down[KEY_STRINGS];
    uint64_t key_up[KEY_STRINGS];

    uint64_t scancode_pressed[SCANCODE_STRINGS];
    uint64_t scancode_down[SCANCODE_STRINGS];
    uint64_t scancode_up[SCANCODE_STRINGS];

    uint64_t extended_pressed[EXTENDED_STRINGS];
    uint64_t extended_down[EXTENDED_STRINGS];
    uint64_t extended_up[EXTENDED_STRINGS];
} Keys;


void keys_clear_events(Keys *keys);
void key_update(Keys *keys, SDL_Keycode code, bool down);
bool key_get_pressed(Keys *keys, SDL_Keycode code);
bool key_get_down(Keys *keys, SDL_Keycode code);
bool key_get_up(Keys *keys, SDL_Keycode code);


#endif
