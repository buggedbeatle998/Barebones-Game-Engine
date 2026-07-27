#ifndef STRUCT_SPRITE
#define STRUCT_SPRITE


#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


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


#endif
