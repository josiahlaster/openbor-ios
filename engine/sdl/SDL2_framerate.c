/*
 * SDL2_framerate.c: Framerate manager for SDL2
 */

#include "SDL2_framerate.h"

void SDL_initFramerate(FPSmanager *manager)
{
    if (!manager) return;
    manager->framecount = 0;
    manager->rate = FPS_DEFAULT;
    manager->rateticks = 1000.0f / (float)FPS_DEFAULT;
    manager->baseticks = SDL_GetTicks();
    manager->lastticks = manager->baseticks;
}

int SDL_setFramerate(FPSmanager *manager, Uint32 rate)
{
    if (!manager) return -1;
    if (rate >= FPS_LOWER_LIMIT && rate <= FPS_UPPER_LIMIT) {
        manager->framecount = 0;
        manager->rate = rate;
        manager->rateticks = 1000.0f / (float)rate;
        return 0;
    }
    return -1;
}

int SDL_getFramerate(FPSmanager *manager)
{
    if (!manager) return -1;
    return (int)manager->rate;
}

Uint32 SDL_framerateDelay(FPSmanager *manager)
{
    Uint32 currentticks;
    Uint32 targetticks;
    Uint32 thedelay;
    Uint32 time_passed;

    if (!manager) return 0;

    manager->framecount++;
    currentticks = SDL_GetTicks();
    time_passed = currentticks - manager->lastticks;
    manager->lastticks = currentticks;
    targetticks = manager->baseticks + (Uint32)((float)manager->framecount * manager->rateticks);

    if (currentticks <= targetticks) {
        thedelay = targetticks - currentticks;
        SDL_Delay(thedelay);
    } else {
        manager->framecount = 0;
        manager->baseticks = SDL_GetTicks();
    }

    return time_passed;
}
