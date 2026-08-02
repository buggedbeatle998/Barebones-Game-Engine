#include "../text.h"


#include "../../engine/sprites.h"
#include <stdio.h>


void draw_text(Sprites *sprites, const Font *font, size_t len, char text[static 1], uint delay, int sep_x, int sep_y, int x, int y, int scale_x, int scale_y) {
    int start = sprites_len(sprites);
    int i = 0;
    int row = 0;
    int row_start = 0;
    for (; text[i] != '\0' && ((len <= 0) ^ (i < len)); ++i) {
        if (text[i] == '\n') {
            row_start = i + 1;
            ++row;
            continue;
        }
        sprite_append(sprites, font->tex_i,
                ((text[i] - ' ') % font->cols) * (font->glyph_w + font->space_x) + font->space_x,
                ((int)(text[i] - ' ') / font->cols) * (font->glyph_h + font->space_y) + font->space_y,
                font->glyph_w, font->glyph_h, scale_x, scale_y,
                x + ((font->glyph_w + font->space_x) * (i - row_start)) * scale_x,
                y - (font->bottom_off + (font->glyph_h + font->space_y) * row) * scale_y);
    }
    update_num(sprites);
    update_range(sprites, start, i - row, true, true, true, false);
}
