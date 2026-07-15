#ifndef FUNC_GRAPHICS
#define FUNC_GRAPHICS


#include <stdlib.h>

#include "sprite_struct.h"

#include "../include/glad/glad.h"
#include <SDL3/SDL.h>

#define MAX_SPRITES 1000
#define tex_w 1280
#define tex_h 960


typedef struct {
    float main_cam[6];
    uint8_t velo;
    SDL_Window *window;
    SDL_Renderer *screen;
    GLuint render;
    Sprites *sprites;
    size_t num_buffers;
    GLuint buffers[100];
    size_t num_textures;
    GLuint textures[100];
} Graphics_Data;


int graphics_init(Graphics_Data *data, const size_t num_sheets, const char *sheet_files[static 1]);
int graphics_step(Graphics_Data *data);
void graphics_destroy(Graphics_Data *data);


#endif
