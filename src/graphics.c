#include "graphics.h"


#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#define SPEED 0.05f

#define rand_float(lower, upper) ((float)rand() / RAND_MAX * ((upper) - (lower)) + (lower))


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


static const float vertices[8] = {
    0.f, -1.f,
    0.f, 0.f,
    1.f, -1.f,
    1.f, 0.f,
};


static const GLuint tex_loc = 0;
static const GLuint vpos_loc = 0;
static const GLuint mat_loc = 1;
static const GLuint texi_loc = 5;
static const GLuint texquad_loc = 6;

static const GLuint cam_loc = 1;


static uint8_t *compile_sprites(const size_t width, const size_t height, const size_t num, const char *files[static 1]);

static GLuint load_shd(const char *filename, GLenum type, const char *entry);
static void shd_loadatt(GLuint program, const char *filename, GLenum type, const char *entry);

static GLuint make_draw_tex(Graphics_Data *gdata, const size_t dtex_w, const size_t dtex_h, const GLenum texture);
static GLuint make_sheet_tex(Graphics_Data *gdata, const size_t stex_w, const size_t stex_h, const size_t num, const GLenum texture, const void *pixels);
static GLuint make_buffer(Graphics_Data *gdata, const GLenum type, const GLenum usage, const size_t size, const void *data);

static uint8_t handle_keys(SDL_Event ev, uint8_t velo);
//static void handle_move(float cam[6], uint8_t velo);



int graphics_init(Graphics_Data *data, const size_t num_sheets, const char *sheet_files[static 1]) {
    if (!SDL_Init(SDL_INIT_VIDEO))
        return -1;
   
    data->window = SDL_CreateWindow("Hello, World!", tex_w, tex_h, SDL_WINDOW_OPENGL);
    if (!data->window) {
        SDL_Quit();
        return -1;
    }
    data->screen = SDL_CreateRenderer(data->window, "Hi");
    SDL_SetRenderVSync(data->screen, 1);

    SDL_GLContext context = SDL_GL_CreateContext(data->window);
    SDL_GL_MakeCurrent(data->window, context);
    SDL_GL_SetSwapInterval(1);
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

    int temp_w;
    int temp_h;
    free(stbi_load(sheet_files[0], &temp_w, &temp_h, NULL, 4));
    uint8_t *pixels = compile_sprites(temp_w, temp_h, num_sheets, sheet_files);
    if (pixels == NULL) {
        SDL_DestroyRenderer(data->screen);
        SDL_DestroyWindow(data->window);
        SDL_Quit();
        return -1;
    }
    GLuint tex_sheet = make_sheet_tex(data, temp_w, temp_h, num_sheets, GL_TEXTURE0, pixels);
    free(pixels);

    data->sprites->buff_indir = make_buffer(data, GL_DRAW_INDIRECT_BUFFER, GL_STATIC_READ,
            sizeof(drawarrindircmd), &(drawarrindircmd){4, data->sprites->num_sprites, 0, 0});
    
    GLuint buff_verts = make_buffer(data, GL_ARRAY_BUFFER, GL_STATIC_DRAW,
            sizeof(vertices), vertices);
    
    data->sprites->buff_mats = make_buffer(data, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW,
            sizeof(float) * 16 * MAX_SPRITES, NULL);
    
    data->sprites->buff_texis = make_buffer(data, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW,
            sizeof(uint) * 1 * MAX_SPRITES, NULL);
    
    data->sprites->buff_quads = make_buffer(data, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW,
            sizeof(uint) * 4 * MAX_SPRITES, NULL);
    
    data->render = glad_glCreateProgram();
    shd_loadatt(data->render, "../shd/texture.vert.spv", GL_VERTEX_SHADER, "main");
    shd_loadatt(data->render, "../shd/texture.frag.spv", GL_FRAGMENT_SHADER, "main");
    glad_glLinkProgram(data->render);
   
    glad_glBindBuffer(GL_ARRAY_BUFFER, buff_verts);
    glad_glEnableVertexAttribArray(vpos_loc);
    glad_glVertexAttribPointer(vpos_loc, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void *)0);

    glad_glBindBuffer(GL_ARRAY_BUFFER, data->sprites->buff_mats);
    for (int i = 0; i < 4; ++i) {
        glad_glEnableVertexAttribArray(mat_loc + i);
        glad_glVertexAttribPointer(mat_loc + i, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 16, (void *)(sizeof(float) * i * 4));
        glad_glVertexAttribDivisor(mat_loc + i, 1);
    }

    glad_glBindBuffer(GL_ARRAY_BUFFER, data->sprites->buff_texis);
    glad_glEnableVertexAttribArray(texi_loc);
    glad_glVertexAttribIPointer(texi_loc, 1, GL_UNSIGNED_INT, sizeof(uint) * 1, (void *)0);
    glad_glVertexAttribDivisor(texi_loc, 1);

    glad_glBindBuffer(GL_ARRAY_BUFFER, data->sprites->buff_quads);
    glad_glEnableVertexAttribArray(texquad_loc);
    glad_glVertexAttribIPointer(texquad_loc, 4, GL_UNSIGNED_INT, sizeof(uint) * 4, (void *)0);
    glad_glVertexAttribDivisor(texquad_loc, 1);

    glad_glUseProgram(data->render);
    glad_glUniform1i(tex_loc, 0);

    glad_glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    data->velo = 0U;
    return 0;
}


int graphics_step(Graphics_Data *data) {
    int width, height;
    int stop = 0;

    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        switch (ev.type) {
            case SDL_EVENT_QUIT:
                stop = 1;
                break;
            
            case SDL_EVENT_KEY_DOWN:
            case SDL_EVENT_KEY_UP:
                data->velo = handle_keys(ev, data->velo);
                break;

            default:
                break;
        }
    }
    //if (data->velo)
    //    handle_move(data->main_cam, data->velo);

    SDL_RenderClear(data->screen);
    SDL_GetWindowSize(data->window, &width, &height);
    glad_glViewport(0, 0, width, height);

    glad_glUseProgram(data->render);

    glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glad_glDrawArraysIndirect(GL_TRIANGLE_STRIP, 0);
    //glad_glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 1, 1000);
    
    SDL_RenderPresent(data->screen);

    SDL_GL_SwapWindow(data->window);

    //time_t tend = SDL_GetPerformanceCounter();
    //float telapsed = (float)(tend - tstart) / SDL_GetPerformanceFrequency() * 1000;
    //SDL_Delay(floor(stpf - telapsed));

    return stop;
}


void graphics_destroy(Graphics_Data *data) {
    free(data->sprites->tex_quad);
    free(data->sprites->tex_i);
    free(data->sprites->mat_pos);
    glad_glDeleteProgram(data->render);
    glad_glDeleteTextures(data->num_textures, data->textures);
    glad_glDeleteBuffers(data->num_buffers, data->buffers);
    SDL_DestroyRenderer(data->screen);
    SDL_DestroyWindow(data->window);
    SDL_Quit();
}


static uint8_t *compile_sprites(const size_t width, const size_t height, const size_t num, const char *files[static 1]) {
    size_t full_size = width * height * 4;
    uint8_t *pixels = malloc(sizeof(uint8_t) * full_size * num);

    for (int i = 0; i < num; ++i) {
        int checkw;
        int checkh;
        uint8_t *temp = stbi_load(files[i], &checkw, &checkh, NULL, 4);
        if (checkw != width || checkh != height) {
            free(temp);
            free(pixels);
            return NULL;
        }
        memcpy(pixels + sizeof(uint8_t) * full_size * i, temp, sizeof(uint8_t) * full_size);
        free(temp);
    }

    return pixels;
}


static GLuint load_shd(const char *filename, GLenum type, const char *entry) {
    FILE *f = fopen(filename, "rb");
    fseek(f, 0, SEEK_END);
    const size_t len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buff = malloc(len);
    fread(buff, len, 1, f);
    fclose(f);

    const GLuint shd = glad_glCreateShader(type);
    glad_glShaderBinary(1, &shd, GL_SHADER_BINARY_FORMAT_SPIR_V, (const void *)buff, len);
    glad_glSpecializeShader(shd, entry, 0, NULL, NULL);
    
    GLint compiled;
    glad_glGetShaderiv(shd, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE) {
        GLint shd_len;
        glad_glGetShaderiv(shd, GL_INFO_LOG_LENGTH, &shd_len);

        if (shd_len > 0) {
            GLchar *log = malloc(sizeof(GLchar) * shd_len);
            glad_glGetShaderInfoLog(shd, shd_len, &shd_len, log);
            free(log);
        }
    }
    free(buff);

    return shd;
}


static void shd_loadatt(GLuint program, const char *filename, GLenum type, const char *entry) {
    const GLuint shd = load_shd(filename, type, entry);

    glad_glAttachShader(program, shd);

    glad_glDeleteShader(shd);
}


static GLuint make_draw_tex(Graphics_Data *gdata, const size_t dtex_w, const size_t dtex_h, const GLenum texture) {
    GLuint tex;
    glad_glGenTextures(1, &tex);
    glad_glActiveTexture(texture);
    glad_glBindTexture(GL_TEXTURE_2D, tex);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glad_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, dtex_w, dtex_h, 0, GL_RGBA, GL_FLOAT, NULL);
    glad_glBindImageTexture(0, tex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    
    gdata->textures[gdata->num_textures++] = tex;
    return tex;
}


static GLuint make_sheet_tex(Graphics_Data *gdata, const size_t stex_w, const size_t stex_h, const size_t num, const GLenum texture, const void *pixels) {
    GLuint tex;
    glad_glGenTextures(1, &tex);
    glad_glActiveTexture(texture);
    glad_glBindTexture(GL_TEXTURE_3D, tex);
    glad_glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glad_glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glad_glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, stex_w, stex_h, num, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glad_glBindImageTexture(0, tex, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    
    gdata->textures[gdata->num_textures++] = tex;
    return tex;
}


static GLuint make_buffer(Graphics_Data *gdata, const GLenum type, const GLenum usage, const size_t size, const void *data) {
    GLuint buff;
    glad_glGenBuffers(1, &buff);
    glad_glBindBuffer(type, buff);
    glad_glBufferData(type, size, data, usage);
    
    gdata->buffers[gdata->num_buffers++] = buff;
    return buff;
}


static uint8_t handle_keys(SDL_Event ev, uint8_t velo) {
    switch (ev.key.key) {
        case SDLK_LEFT:
            velo &= ~1U;
            velo |= ev.key.down;
            break;
        case SDLK_RIGHT:
            velo &= ~(1U << 4);
            velo |= ev.key.down << 4;
            break;
        case SDLK_LSHIFT:
            velo &= ~(1U << 1);
            velo |= ev.key.down << 1;
            break;
        case SDLK_SPACE:
            velo &= ~(1U << 5);
            velo |= ev.key.down << 5;
            break;
        case SDLK_DOWN:
            velo &= ~(1U << 2);
            velo |= ev.key.down << 2;
            break;
        case SDLK_UP:
            velo &= ~(1U << 6);
            velo |= ev.key.down << 6;
            break;
        default:
            break;
    }

    return velo;
}
