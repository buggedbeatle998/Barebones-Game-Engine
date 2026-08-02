#include "public_structs.h"


static uint32_t idx_mask = (1U << 6) - 1;



void keys_clear_events(Keys *keys) {
    for (int i = 0; i < KEY_STRINGS; ++i)
        keys->key_down[i] = 0ULL;
    for (int i = 0; i < KEY_STRINGS; ++i)
        keys->key_up[i] = 0ULL;
    
    for (int i = 0; i < SCANCODE_STRINGS; ++i)
        keys->scancode_down[i] = 0ULL;
    for (int i = 0; i < SCANCODE_STRINGS; ++i)
        keys->scancode_up[i] = 0ULL;
    
    for (int i = 0; i < EXTENDED_STRINGS; ++i)
        keys->extended_down[i] = 0ULL;
    for (int i = 0; i < EXTENDED_STRINGS; ++i)
        keys->extended_up[i] = 0ULL;
}


void key_update(Keys *keys, SDL_Keycode code, bool down) {
    int num, idx;
    if (code & SDLK_SCANCODE_MASK) {
        code &= ~SDLK_SCANCODE_MASK;
        num = code >> 6;
        idx = code & idx_mask;
        keys->scancode_pressed[num] &= ~(1U << idx);
        keys->scancode_pressed[num] |= (uint32_t)down << idx;
        keys->scancode_down[num] |= (uint32_t)down << idx;
        keys->scancode_up[num] |= (uint32_t)(!down) << idx;
    } else if (code & SDLK_EXTENDED_MASK) {
        code &= ~SDLK_EXTENDED_MASK;
        num = code >> 6;
        idx = code & idx_mask;
        keys->extended_pressed[num] &= ~(1U << idx);
        keys->extended_pressed[num] |= (uint32_t)down << idx;
        keys->extended_down[num] |= (uint32_t)down << idx;
        keys->extended_up[num] |= (uint32_t)(!down) << idx;
    } else {
        num = code >> 6;
        idx = code & idx_mask;
        keys->key_pressed[num] &= ~(1U << idx);
        keys->key_pressed[num] |= (uint32_t)down << idx;
        keys->key_down[num] |= (uint32_t)down << idx;
        keys->key_up[num] |= (uint32_t)(!down) << idx;
    }
}


bool key_get_pressed(Keys *keys, SDL_Keycode code) {
    int num, idx;
    if (code & SDLK_SCANCODE_MASK) {
        code &= ~SDLK_SCANCODE_MASK;
        num = code >> 6;
        idx = code & idx_mask;
        return (keys->scancode_pressed[num] & (1U << idx)) != 0ULL;
    } else if (code & SDLK_EXTENDED_MASK) {
        code &= ~SDLK_EXTENDED_MASK;
        num = code >> 6;
        idx = code & idx_mask;
        return (keys->extended_pressed[num] & (1U << idx)) != 0ULL;
    } else {
        num = code >> 6;
        idx = code & idx_mask;
        return (keys->key_pressed[num] & (1U << idx)) != 0ULL;
    }
}


bool key_get_down(Keys *keys, SDL_Keycode code) {
    int num, idx;
    if (code & SDLK_SCANCODE_MASK) {
        code &= ~SDLK_SCANCODE_MASK;
        num = code >> 6;
        idx = code & idx_mask;
        return (keys->scancode_down[num] & (1U << idx)) != 0ULL;
    } else if (code & SDLK_EXTENDED_MASK) {
        code &= ~SDLK_EXTENDED_MASK;
        num = code >> 6;
        idx = code & idx_mask;
        return (keys->extended_down[num] & (1U << idx)) != 0ULL;
    } else {
        num = code >> 6;
        idx = code & idx_mask;
        return (keys->key_down[num] & (1U << idx)) != 0ULL;
    }
}


bool key_get_up(Keys *keys, SDL_Keycode code) {
    int num, idx;
    if (code & SDLK_SCANCODE_MASK) {
        code &= ~SDLK_SCANCODE_MASK;
        num = code >> 6;
        idx = code & idx_mask;
        return (keys->scancode_up[num] & (1U << idx)) != 0ULL;
    } else if (code & SDLK_EXTENDED_MASK) {
        code &= ~SDLK_EXTENDED_MASK;
        num = code >> 6;
        idx = code & idx_mask;
        return (keys->extended_up[num] & (1U << idx)) != 0ULL;
    } else {
        num = code >> 6;
        idx = code & idx_mask;
        return (keys->key_up[num] & (1U << idx)) != 0ULL;
    }
}
