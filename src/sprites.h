#ifndef FUNC_SPRITES
#define FUNC_SPRITES


#include "sprite_struct.h"


void sprite_append(Sprites *sprites, int32_t sheet_num, uint32_t quadx, uint32_t quady, uint32_t sizex, uint32_t sizey, int scalex, int scaley, int posx, int posy);
void sprite_pop(Sprites *sprites, int num);

void matrix_set(Sprites *sprites, int idx, int scalex, int scaley, int posx, int posy);
void matrix_translate(Sprites *sprites, int idx, int changex, int changey);
void matrix_scale(Sprites *sprites, int idx, float scalex, float scaley);
void matrix_rotate(Sprites *sprites, int idx, float angle);

void sheet_set(Sprites *sprites, int idx, int32_t sheet_num, uint32_t quadx, uint32_t quady, uint32_t sizex, uint32_t size_y);
void sheet_move(Sprites *sprites, int idx, uint32_t quadx, uint32_t quady, uint32_t sizex, uint32_t size_y);

void update_num(Sprites *sprites);
void update_range(Sprites *sprites, int idx, int len, bool matrix, bool sheet_num, bool tex_quad);


#endif
