#ifdef DEBUG_FRAME_TIME
#include <stdio.h>
#endif

#include "graphics.h"
#include "../world.h"


int main(void) {
    Graphics_Data *graphics_data = &(Graphics_Data){};
    graphics_data->sprites = &(Sprites){
        .num_sprites=0,
        .mat_pos=malloc(sizeof(float) * 16 * MAX_SPRITES),
        .tex_i=malloc(sizeof(int32_t) * 1 * MAX_SPRITES),
        .tex_quad=malloc(sizeof(uint32_t) * 4 * MAX_SPRITES),
        .blend=malloc(sizeof(float) * 4 * MAX_SPRITES)
    };

    Keys *keys = &(Keys){};

    int exit_code;
    if ((exit_code = graphics_init(graphics_data, sizeof(sheet_files) / sizeof(char *), sheet_files)))
        return exit_code;
    if ((exit_code = world_init(graphics_data->sprites)))
        return exit_code;
    
    const uint64_t tpf = SDL_GetPerformanceFrequency() / FPS;
#ifdef DEBUG_FRAME_TIME
    printf("%li", SDL_GetPerformanceFrequency());
    time_t lag[10000];
    int ticks[10000];
    int frames = 0;
    int tick_count = 0;
#endif
    time_t tstart, tend;
    int width, height;
    while (!exit_code) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            switch (ev.type) {
                case SDL_EVENT_QUIT:
                    exit_code = 1;
                    break;

                case SDL_EVENT_WINDOW_RESIZED:
                    SDL_GetWindowSize(graphics_data->window, &width, &height);
                    glad_glViewport(0, 0, width, height);
                    break;
                
                case SDL_EVENT_KEY_DOWN:
                case SDL_EVENT_KEY_UP:
                    key_update(keys, ev.key.key, ev.key.down);
                    break;

                default:
                    break;
            }
        }
        if (exit_code)
            break;

#ifdef DEBUG_FRAME_TIME
        ++tick_count;
#endif
        // Only at 1/60s intervals
        tend = SDL_GetPerformanceCounter();
        if (tend - tstart >= tpf) {
#ifdef DEBUG_FRAME_TIME
            ticks[frames] = tick_count;
            lag[frames++] = tend - tstart - tpf;
            tick_count = 0;
#endif
            tstart = tend;
            if ((exit_code = graphics_step(graphics_data)))
                break;
            exit_code = world_step(graphics_data->sprites, keys);
            keys_clear_events(keys);
        }
    };
#ifdef DEBUG_FRAME_TIME
    for (int i = 0; i < frames; ++i)
        printf("%li %i\n", lag[i], ticks[i]);
#endif

    graphics_destroy(graphics_data);
    world_destroy();
    
    if (exit_code == 1)
        return 0;
    return exit_code;
}
