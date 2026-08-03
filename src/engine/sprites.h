#ifndef FUNC_SPRITES
#define FUNC_SPRITES


#include "public_structs.h"


void sprite_append(Sprites *sprites, int32_t sheet_num, uint32_t quadx, uint32_t quady, uint32_t sizex, uint32_t sizey, int scalex, int scaley, int posx, int posy);
void sprite_pop(Sprites *sprites, int num);
int  sprites_len(Sprites *sprites);

void matrix_set(Sprites *sprites, int idx, int scalex, int scaley, int posx, int posy);
void matrix_set_pos(Sprites *sprites, int idx, int posx, int posy);
void matrix_set_scale(Sprites *sprites, int idx, int scalex, int scaley);
void matrix_set_angle(Sprites *sprites, int idx, float angle);

int  matrix_get_posx(Sprites *sprites, int idx);
int  matrix_get_posy(Sprites *sprites, int idx);
int  matrix_get_scalex(Sprites *sprites, int idx);
int  matrix_get_scaley(Sprites *sprites, int idx);
int  matrix_get_angle(Sprites *sprites, int idx, float angle);

void matrix_translate(Sprites *sprites, int idx, int changex, int changey);
void matrix_scale(Sprites *sprites, int idx, float scalex, float scaley);
void matrix_rotate(Sprites *sprites, int idx, float angle);

void sheet_set(Sprites *sprites, int idx, int32_t sheet_num, uint32_t quadx, uint32_t quady, uint32_t sizex, uint32_t size_y);
void sheet_move(Sprites *sprites, int idx, uint32_t quadx, uint32_t quady, uint32_t sizex, uint32_t size_y);

void blend_set(Sprites *sprites, int idx, float red, float green, float blue, float alpha);
void blend_blend(Sprites *sprites, int idx, float red, float green, float blue, float alpha);

void update_num(Sprites *sprites);
void update_range(Sprites *sprites, int idx, int len, bool matrix, bool sheet_num, bool tex_quad, bool blend);


#endif
