/*
 * SDL2_framerate.h: Framerate manager for SDL2
 * Standard interface compatible with SDL2_gfx
 */

#ifndef SDL2_FRAMERATE_H
#define SDL2_FRAMERATE_H

#include <SDL.h>

#define FPS_UPPER_LIMIT 200
#define FPS_LOWER_LIMIT 1
#define FPS_DEFAULT     30

typedef struct {
    Uint32 framecount;
    float rateticks;
    Uint32 baseticks;
    Uint32 lastticks;
    Uint32 rate;
} FPSmanager;

#ifdef __cplusplus
extern "C" {
#endif

void SDL_initFramerate(FPSmanager *manager);
int SDL_setFramerate(FPSmanager *manager, Uint32 rate);
int SDL_getFramerate(FPSmanager *manager);
Uint32 SDL_framerateDelay(FPSmanager *manager);

#ifdef __cplusplus
}
#endif

#endif /* SDL2_FRAMERATE_H */
