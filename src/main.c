#include "graphics.h"
#include <stdio.h>


int main(void) {
    const char *sheet_files[] = {"../assets/tex_alien.png", "../assets/tex_test.png"};
    Graphics_Data *graphics_data = &(Graphics_Data){{0.f, 10.f, 0.f, 0.f, 0.f, 1.f}};
    int exit_code;
    if ((exit_code = graphics_init(graphics_data, sizeof(sheet_files) / sizeof(char *), sheet_files)))
        return exit_code;

    while (!(exit_code = graphics_step(graphics_data)));

    graphics_destroy (graphics_data);

    return exit_code;
}
