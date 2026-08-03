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


int sprites_len(Sprites *sprites) {
    return sprites->num_sprites;
}


void matrix_set(Sprites *sprites, int idx, int scalex, int scaley, int posx, int posy) {
    trans_base(sprites->mat_pos + 16 * idx, (float)scalex / tex_w, (float)(-scaley) / tex_h, (float)posx / tex_w, (float)posy / tex_h);
}


void matrix_set_pos(Sprites *sprites, int idx, int posx, int posy) {
    trans_set_pos(sprites->mat_pos + 16 * idx, (float)posx / tex_w, (float)posy / tex_h);
}


void matrix_set_scale(Sprites *sprites, int idx, int scalex, int scaley) {
    trans_set_scale(sprites->mat_pos + 16 * idx, (float)scalex / tex_w, (float)(-scaley) / tex_h);
}


void matrix_set_angle(Sprites *sprites, int idx, float angle) {
    trans_set_angle(sprites->mat_pos + 16 * idx, angle);
}


int matrix_get_posx(Sprites *sprites, int idx) {
    return trans_get_posx(sprites->mat_pos + 16 * idx);
}


int matrix_get_posy(Sprites *sprites, int idx) {
    return trans_get_posy(sprites->mat_pos + 16 * idx);
}


int matrix_get_scalex(Sprites *sprites, int idx) {
    return trans_get_scalex(sprites->mat_pos + 16 * idx);
}


int matrix_get_scaley(Sprites *sprites, int idx) {
    return trans_get_scaley(sprites->mat_pos + 16 * idx);
}


int matrix_get_angle(Sprites *sprites, int idx, float angle) {
    return trans_get_angle(sprites->mat_pos + 16 * idx);
}


void matrix_translate(Sprites *sprites, int idx, int changex, int changey) {
    trans_translate(sprites->mat_pos + 16 * idx, (float)changex / tex_w, (float)changey / tex_h);
}


void matrix_scale(Sprites *sprites, int idx, float scalex, float scaley) {
    trans_scale(sprites->mat_pos + 16 * idx, scalex, scaley);
}


void matrix_rotate(Sprites *sprites, int idx, float angle) {
    trans_rotate(sprites->mat_pos + 16 * idx, angle);
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


void blend_set(Sprites *sprites, int idx, float red, float green, float blue, float alpha) {
    sprites->blend[idx * 4 + 0] = red;
    sprites->blend[idx * 4 + 1] = green;
    sprites->blend[idx * 4 + 2] = blue;
    sprites->blend[idx * 4 + 3] = alpha;
}


void blend_blend(Sprites *sprites, int idx, float red, float green, float blue, float alpha) {
    sprites->blend[idx * 4 + 0] *= red;
    sprites->blend[idx * 4 + 1] *= green;
    sprites->blend[idx * 4 + 2] *= blue;
    sprites->blend[idx * 4 + 3] *= alpha;
}


void update_num(Sprites *sprites) {
    glad_glBindBuffer(GL_DRAW_INDIRECT_BUFFER, sprites->buff_indir);
    glad_glBufferSubData(GL_DRAW_INDIRECT_BUFFER, sizeof(uint) * 1, sizeof(uint), &sprites->num_sprites);
}


void update_range(Sprites *sprites, int idx, int len, bool matrix, bool sheet_num, bool tex_quad, bool blend) {
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
    if (blend) {
        glad_glBindBuffer(GL_ARRAY_BUFFER, sprites->buff_blends);
        glad_glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * 4 * idx, sizeof(float) * 4 * len, sprites->blend + 4 * idx);
    }
}
