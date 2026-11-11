/*
 * OpenBOR - iOS / Mobile Virtual Touchscreen Controls
 */

#ifndef TOUCHCONTROLS_H
#define TOUCHCONTROLS_H

#include <SDL.h>

#define MAX_TOUCH_BUTTONS 13
#define MAX_TOUCH_POINTERS 8

// Touch button indices
enum {
    TOUCH_BTN_UP = 0,
    TOUCH_BTN_RIGHT,
    TOUCH_BTN_DOWN,
    TOUCH_BTN_LEFT,
    TOUCH_BTN_ATTACK1,
    TOUCH_BTN_ATTACK2,
    TOUCH_BTN_JUMP,
    TOUCH_BTN_SPECIAL,
    TOUCH_BTN_START,
    TOUCH_BTN_ESC,
    TOUCH_BTN_ATTACK3,
    TOUCH_BTN_ATTACK4,
    TOUCH_BTN_SCREENSHOT
};

typedef struct {
    float x;      // Center X coordinate in screen pixels
    float y;      // Center Y coordinate in screen pixels
    float radius; // Effective touch radius in pixels
    int state;    // 1 = pressed, 0 = released
    SDL_Rect src_rect;
    SDL_Rect dst_rect;
} TouchButton;

typedef struct {
    float scale;         // Button scale factor (0.5 to 2.0, default 1.0)
    int alpha_normal;    // Default opacity (0 - 255, default 64)
    int alpha_pressed;   // Pressed opacity (0 - 255, default 128)
    int visible;         // 1 = visible, 0 = hidden (e.g. when physical controller connected)
    Uint32 auto_fade_ticks; // Tick count when touch controls should fade
} TouchConfig;

#ifdef __cplusplus
extern "C" {
#endif

void touch_init(SDL_Renderer *renderer, int screen_w, int screen_h);
void touch_update_layout(int screen_w, int screen_h, float safe_left, float safe_right, float safe_top, float safe_bottom);
void touch_handle_event(const SDL_Event *event);
void touch_render(SDL_Renderer *renderer);
void touch_apply_inputs(Uint8 *keystate);
void touch_set_visible(int visible);
void touch_set_scale(float scale);
void touch_set_opacity(int normal, int pressed);

#ifdef __cplusplus
}
#endif

#endif /* TOUCHCONTROLS_H */
