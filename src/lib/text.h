#ifndef LIB_TEXT
#define LIB_TEXT


#include <stdlib.h>

#include "../engine/public_structs.h"


typedef struct {
    uint tex_i;
    int font_x;
    int font_y;
    uint glyph_w;
    uint glyph_h;
    uint space_x;
    uint space_y;
    uint bottom_off;
    int cols;
} Font;


void draw_text(Sprites *sprites, const Font *font, size_t len, char text[static 1], uint delay, int sep_x, int sep_y, int x, int y, int scale_x, int scale_y);


#endif
