#ifndef FUNC_WORLD
#define FUNC_WORLD


#include "engine/public_structs.h"

#define FPS 120


static const char *sheet_files[] = {"../assets/tex_fonts.png", "../assets/tex_test.png"};


int world_init(Sprites *sprites);
int world_step(Sprites *sprites, Keys *keys);
void world_destroy(void);


#endif
