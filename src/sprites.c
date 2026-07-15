#include "sprites.h"

#include "graphics.h"
#include "linmaths.h"


void sprite_append(Sprites *sprites, int32_t sheet_num, uint32_t quadx, uint32_t quady, uint32_t sizex, uint32_t sizey, int scalex, int scaley, int posx, int posy) {
    sheet_set(sprites, sprites->num_sprites, sheet_num, quadx, quady, sizex, sizey);
    matrix_set(sprites, sprites->num_sprites++, scalex, scaley, posx, posy);
}


void sprite_pop(Sprites *sprites, int num) {
    sprites->num_sprites -= num;
}


void matrix_set(Sprites *sprites, int idx, int scalex, int scaley, int posx, int posy) {
    trans_base(sprites->mat_pos + 16 * idx, scalex, scaley, (float)posx / tex_w, (float)posy / tex_h);
}


void matrix_translate(Sprites *sprites, int idx, float changex, float changey) {
    trans_translate(sprites->mat_pos + sizeof(float) * 16 * idx, changex, changey);
}


void matrix_scale(Sprites *sprites, int idx, float scalex, float scaley) {
    trans_scale(sprites->mat_pos + sizeof(float) * 16 * idx, scalex, scaley);
}


void matrix_rotate(Sprites *sprites, int idx, float angle) {
    trans_rotate(sprites->mat_pos + sizeof(float) * 16 * idx, angle);
}


void sheet_set(Sprites *sprites, int idx, int32_t sheet_num, uint32_t quadx, uint32_t quady, uint32_t sizex, uint32_t sizey) {
    sprites->tex_i[idx] = sheet_num;
    sheet_move(sprites, idx, quadx, quady, sizex, sizey);
}


void sheet_move(Sprites *sprites, int idx, uint32_t quadx, uint32_t quady, uint32_t sizex, uint32_t sizey) {
    sprites->tex_quad[idx * 4 + 0] = quadx;
    sprites->tex_quad[idx * 4 + 1] = quady;
    sprites->tex_quad[idx * 4 + 2] = sizex;
    sprites->tex_quad[idx * 4 + 3] = sizey;
}


void update_num(Sprites *sprites) {
    glad_glBindBuffer(GL_DRAW_INDIRECT_BUFFER, sprites->buff_indir);
    glad_glBufferSubData(GL_DRAW_INDIRECT_BUFFER, sizeof(uint) * 1, sizeof(uint), &sprites->num_sprites);
}


void update_range(Sprites *sprites, int idx, int len, bool matrix, bool sheet_num, bool tex_quad) {
    if (matrix) {
        glad_glBindBuffer(GL_ARRAY_BUFFER, sprites->buff_mats);
        glad_glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * 16 * idx, sizeof(float) * 16 * len, sprites->mat_pos + 16 * idx);
    }
    if (sheet_num) {
        glad_glBindBuffer(GL_ARRAY_BUFFER, sprites->buff_texis);
        glad_glBufferSubData(GL_ARRAY_BUFFER, sizeof(uint) * 1 * idx, sizeof(uint) * 1 * len, sprites->tex_i + 1 * idx);
    }
    if (tex_quad) {
        glad_glBindBuffer(GL_ARRAY_BUFFER, sprites->buff_quads);
        glad_glBufferSubData(GL_ARRAY_BUFFER, sizeof(uint) * 4 * idx, sizeof(uint) * 4 * len, sprites->tex_quad + 4 * idx);
    }
}
