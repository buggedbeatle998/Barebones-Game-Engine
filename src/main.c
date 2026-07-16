#include "graphics.h"
#include "world.h"

#define FPS 60


int main(void) {
    const char *sheet_files[] = {"../assets/tex_alien.png", "../assets/tex_test.png"};
    Graphics_Data *graphics_data = &(Graphics_Data){{0.f, 10.f, 0.f, 0.f, 0.f, 1.f}};
    graphics_data->sprites = &(Sprites){0,
        malloc(sizeof(float) * 16 * MAX_SPRITES), malloc(sizeof(int32_t) * MAX_SPRITES), malloc(sizeof(uint32_t) * 4 * MAX_SPRITES)};
    int exit_code;
    if ((exit_code = graphics_init(graphics_data, sizeof(sheet_files) / sizeof(char *), sheet_files)))
        return exit_code;
    if ((exit_code = world_init(graphics_data->sprites)))
        return exit_code;
    
    const uint64_t tpf = SDL_GetPerformanceFrequency() / FPS;
    time_t tstart, tend;
    while (!exit_code) {
        if ((exit_code = graphics_step(graphics_data)))
            break;
        // wait
        tend = SDL_GetPerformanceCounter();
        if (tend - tstart >= tpf) {
            tstart = tend;
            exit_code = world_step(graphics_data->sprites);
        }
    };

    graphics_destroy(graphics_data);
    world_destroy();
    
    if (exit_code == 1)
        return 0;
    return exit_code;
}
