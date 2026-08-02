#ifndef FUNC_WORLD
#define FUNC_WORLD


#include "public_structs.h"


int world_init(Sprites *sprites);
int world_step(Sprites *sprites, Keys *keys);
void world_destroy(void);


#endif
