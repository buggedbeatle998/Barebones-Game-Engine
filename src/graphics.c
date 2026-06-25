#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#include "../include/linmath.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "graphics.h"

#define SPEED 0.05f

static float rand_float(float lower, float upper) {
    return (float)rand() / RAND_MAX * (upper - lower) + lower;
}


//static float vertices[8 * 100];
static const float vertices[8] = {
    -1.f, -1.f,
    -1.f, 1.f,
    1.f, -1.f,
    1.f, 1.f,
};
static float matss[16 * MAX_SPRITES];

static const int tex_w = 1280;
static const int tex_h = 960;

//static const GLint const_bind = 1;

static const GLint tex_loc = 0;
static const GLint vpos_loc = 0;
static const GLint mat_loc = 1;
static const GLint texi_loc = 5;
static const GLint texquad_loc = 6;

static const GLint cam_loc = 1;


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

    data->sprites = &(Sprites){false, 4};
    
    GLuint buff_indir = make_buffer(data, GL_DRAW_INDIRECT_BUFFER, GL_STATIC_READ,
            sizeof(drawarrindircmd), &(drawarrindircmd){4, MAX_SPRITES, 0, 0});
    glad_glBufferSubData(GL_DRAW_INDIRECT_BUFFER, sizeof(uint) * 1, sizeof(uint), &data->sprites->num_sprites);
    
    uint scale = 400;
    for (int i = 0; i < MAX_SPRITES; ++i) {
        vec2 corner = {rand_float(-1, 1 - (float)scale / tex_w), rand_float(-1, 1 - (float)scale / tex_h)};
        data->sprites->mat_pos[i * 16 + 0] = (float)scale / tex_w;
        data->sprites->mat_pos[i * 16 + 1] = 0.f;
        data->sprites->mat_pos[i * 16 + 2] = 0.f;
        data->sprites->mat_pos[i * 16 + 3] = 0.f;
        data->sprites->mat_pos[i * 16 + 4] = 0.f;
        data->sprites->mat_pos[i * 16 + 5] = -(float)scale / tex_h;
        data->sprites->mat_pos[i * 16 + 6] = 0.f;
        data->sprites->mat_pos[i * 16 + 7] = 0.f;
        data->sprites->mat_pos[i * 16 + 8] = 0.f;
        data->sprites->mat_pos[i * 16 + 9] = 0.f;
        data->sprites->mat_pos[i * 16 + 10] = 1.f;
        data->sprites->mat_pos[i * 16 + 11] = 0.f;
        data->sprites->mat_pos[i * 16 + 12] = corner[0];
        data->sprites->mat_pos[i * 16 + 13] = corner[1];
        data->sprites->mat_pos[i * 16 + 14] = 0.f;
        data->sprites->mat_pos[i * 16 + 15] = 1.f;

        data->sprites->tex_i[i] = i & 1;
        
        data->sprites->tex_quad[i * 4 + 0] = 0;
        data->sprites->tex_quad[i * 4 + 1] = 0;
        data->sprites->tex_quad[i * 4 + 2] = 32;
        data->sprites->tex_quad[i * 4 + 3] = 32;
    }

    GLuint buff_verts = make_buffer(data, GL_ARRAY_BUFFER, GL_STATIC_DRAW,
            sizeof(vertices), vertices);
    
    GLuint buff_mats = make_buffer(data, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW,
            sizeof(data->sprites->mat_pos), data->sprites->mat_pos);
    
    GLuint buff_quads = make_buffer(data, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW,
            sizeof(data->sprites->tex_quad), data->sprites->tex_quad);
    
    GLuint buff_texis = make_buffer(data, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW,
            sizeof(data->sprites->tex_i), data->sprites->tex_i);
    
    //GLuint consts = make_buffer(GL_UNIFORM_BUFFER, GL_STATIC_READ,
    //        sizeof(push_consts), &(push_consts){
    //});

    //GLuint comp_indir = make_buffer(GL_DISPATCH_INDIRECT_BUFFER, GL_STATIC_READ,
    //        sizeof(dispindircmd), &(dispindircmd){(tex_w + 31) / 32, (tex_h + 31) / 32, 1});
    
    
    
    //const GLuint compute = glad_glCreateProgram();
    //shd_loadatt(compute, "../shd/raytrace.comp.spv", GL_COMPUTE_SHADER, "main");
    //glad_glLinkProgram(compute);

    data->render = glad_glCreateProgram();
    shd_loadatt(data->render, "../shd/texture.vert.spv", GL_VERTEX_SHADER, "main");
    shd_loadatt(data->render, "../shd/texture.frag.spv", GL_FRAGMENT_SHADER, "main");
    glad_glLinkProgram(data->render);
   
    glad_glBindBuffer(GL_ARRAY_BUFFER, buff_verts);
    glad_glEnableVertexAttribArray(vpos_loc);
    glad_glVertexAttribPointer(vpos_loc, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void *)0);

    glad_glBindBuffer(GL_ARRAY_BUFFER, buff_mats);
    for (int i = 0; i < 4; ++i) {
        glad_glEnableVertexAttribArray(mat_loc + i);
        glad_glVertexAttribPointer(mat_loc + i, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 16, (void *)(sizeof(float) * i * 4));
        glad_glVertexAttribDivisor(mat_loc + i, 1);
    }

    glad_glBindBuffer(GL_ARRAY_BUFFER, buff_quads);
    glad_glEnableVertexAttribArray(texquad_loc);
    glad_glVertexAttribIPointer(texquad_loc, 4, GL_UNSIGNED_INT, sizeof(uint) * 4, (void *)0);
    glad_glVertexAttribDivisor(texquad_loc, 1);

    glad_glBindBuffer(GL_ARRAY_BUFFER, buff_texis);
    glad_glEnableVertexAttribArray(texi_loc);
    glad_glVertexAttribIPointer(texi_loc, 1, GL_UNSIGNED_INT, sizeof(uint) * 1, (void *)0);
    glad_glVertexAttribDivisor(texi_loc, 1);


    //glad_glBindBufferBase(GL_UNIFORM_BUFFER, const_bind, consts);
    
    //glad_glUseProgram(data->compute);
    //glad_glUniform1i(tex_loc, 0);
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
    //time_t tstart = SDL_GetPerformanceCounter();

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
    if (data->velo)
        handle_move(data->main_cam, data->velo);

    SDL_RenderClear(data->screen);
    SDL_GetWindowSize(data->window, &width, &height);
    glad_glViewport(0, 0, width, height);

    //glad_glUseProgram(compute);
    //
    //glad_glUniformMatrix2x3fv(cam_loc, 1, GL_FALSE, main_cam.data);
    //glad_glDispatchComputeIndirect(0);
    //
    //glad_glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    glad_glUseProgram(data->render);

    glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glad_glDrawArraysIndirect(GL_TRIANGLE_STRIP, 0);
    //glad_glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 100);
    
    SDL_RenderPresent(data->screen);

    SDL_GL_SwapWindow(data->window);

    //time_t tend = SDL_GetPerformanceCounter();
    //float telapsed = (float)(tend - tstart) / SDL_GetPerformanceFrequency() * 1000;
    //SDL_Delay(floor(stpf - telapsed));

    return stop;
}


void graphics_destroy(Graphics_Data *data) {
    glad_glDeleteProgram(data->render);
    glad_glDeleteTextures(data->num_textures, data->textures);
    glad_glDeleteBuffers(data->num_buffers, data->buffers);
    SDL_DestroyRenderer(data->screen);
    SDL_DestroyWindow(data->window);
    SDL_Quit();
}


uint8_t *compile_sprites(const size_t width, const size_t height, const size_t num, const char *files[static 1]) {
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


GLuint load_shd(const char *filename, GLenum type, const char *entry) {
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


void shd_loadatt(GLuint program, const char *filename, GLenum type, const char *entry) {
    const GLuint shd = load_shd(filename, type, entry);

    glad_glAttachShader(program, shd);

    glad_glDeleteShader(shd);
}


GLuint make_draw_tex(Graphics_Data *gdata, const size_t tex_w, const size_t tex_h, const GLenum texture) {
    GLuint tex;
    glad_glGenTextures(1, &tex);
    glad_glActiveTexture(texture);
    glad_glBindTexture(GL_TEXTURE_2D, tex);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glad_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tex_w, tex_h, 0, GL_RGBA, GL_FLOAT, NULL);
    glad_glBindImageTexture(0, tex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    
    gdata->textures[gdata->num_textures++] = tex;
    return tex;
}


GLuint make_sheet_tex(Graphics_Data *gdata, const size_t tex_w, const size_t tex_h, const size_t num, const GLenum texture, const void *pixels) {
    GLuint tex;
    glad_glGenTextures(1, &tex);
    glad_glActiveTexture(texture);
    glad_glBindTexture(GL_TEXTURE_3D, tex);
    glad_glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glad_glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glad_glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, tex_w, tex_h, num, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glad_glBindImageTexture(0, tex, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    
    gdata->textures[gdata->num_textures++] = tex;
    return tex;
}


GLuint make_buffer(Graphics_Data *gdata, const GLenum type, const GLenum usage, const size_t size, const void *data) {
    GLuint buff;
    glad_glGenBuffers(1, &buff);
    glad_glBindBuffer(type, buff);
    glad_glBufferData(type, size, data, usage);
    
    gdata->buffers[gdata->num_buffers++] = buff;
    return buff;
}


uint8_t handle_keys(SDL_Event ev, uint8_t velo) {
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


void handle_move(float cam[6], uint8_t velo) {
    velo ^= velo >> 4;
    if (velo & 1) {
        cam[0] += (-0.5f + ((velo >> 4) & 1)) * SPEED;
    }
    velo >>= 1;
    if (velo & 1) {
        cam[1] += (-0.5f + ((velo >> 4) & 1)) * SPEED;
    }
    velo >>= 1;
    if (velo & 1) {
        cam[2] += (-0.5f + ((velo >> 4) & 1)) * SPEED;
    }
}
