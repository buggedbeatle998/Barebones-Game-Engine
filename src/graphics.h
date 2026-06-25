#ifndef FUNC_GRAPHICS
#define FUNC_GRAPHICS


#include <stdlib.h>

#include "../include/glad/glad.h"
#include <SDL3/SDL.h>

#define MAX_SPRITES 1000


typedef struct {
    uint x;
    uint y;
    uint z;
} dispindircmd;


typedef struct {
    uint count;
    uint inst_count;
    uint first;
    uint base_inst;
} drawarrindircmd;


//typedef struct {
//} push_consts;

struct Sprites;

typedef struct {
    float main_cam[6];
    uint8_t velo;
    SDL_Window *window;
    SDL_Renderer *screen;
    GLuint render;
    struct Sprites *sprites;
    size_t num_buffers;
    GLuint buffers[100];
    size_t num_textures;
    GLuint textures[100];
} Graphics_Data;


typedef struct Sprites {
    bool changed;
    size_t num_sprites;
    float mat_pos[16 * MAX_SPRITES];
    int tex_i[MAX_SPRITES];
    uint tex_quad[4 * MAX_SPRITES];
} Sprites;


int graphics_init(Graphics_Data *data, const size_t num_sheets, const char *sheet_files[static 1]);
int graphics_step(Graphics_Data *data);
void graphics_destroy(Graphics_Data *data);

uint8_t *compile_sprites(const size_t width, const size_t height, const size_t num, const char *files[static 1]);

GLuint load_shd(const char *filename, GLenum type, const char *entry);
void shd_loadatt(GLuint program, const char *filename, GLenum type, const char *entry);

GLuint make_draw_tex(Graphics_Data *gdata, const size_t tex_w, const size_t tex_h, const GLenum texture);
GLuint make_sheet_tex(Graphics_Data *gdata, const size_t tex_w, const size_t tex_h, const size_t num, const GLenum texture, const void *pixels);
GLuint make_buffer(Graphics_Data *gdata, const GLenum type, const GLenum usage, const size_t size, const void *data);

uint8_t handle_keys(SDL_Event ev, uint8_t velo);
void handle_move(float cam[6], uint8_t velo);


#endif
