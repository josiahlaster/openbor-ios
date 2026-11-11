/*
 * OpenBOR - iOS / Mobile Virtual Touchscreen Controls Implementation
 */

#include "touchcontrols.h"
#include "button_png_800x480.h"
#include <math.h>
#include <string.h>

extern SDL_Surface *pngToSurface(void *data);

static TouchButton g_buttons[MAX_TOUCH_BUTTONS];
static TouchConfig g_config = {
    .scale = 1.0f,
    .alpha_normal = 64,
    .alpha_pressed = 130,
    .visible = 1,
    .auto_fade_ticks = 0
};

static SDL_Texture *g_button_texture = NULL;
static int g_screen_w = 0;
static int g_screen_h = 0;

typedef struct {
    SDL_FingerID id;
    float x;
    float y;
    int down;
} ActivePointer;

static ActivePointer g_pointers[MAX_TOUCH_POINTERS];

#define _BTN_RECT(col, row) { 144 * (col), 144 * (row), 96, 96 }

static const SDL_Rect g_default_src_rects[MAX_TOUCH_BUTTONS] = {
    [TOUCH_BTN_UP]         = _BTN_RECT(0, 2),
    [TOUCH_BTN_RIGHT]      = _BTN_RECT(1, 2),
    [TOUCH_BTN_DOWN]       = _BTN_RECT(2, 2),
    [TOUCH_BTN_LEFT]       = _BTN_RECT(3, 2),
    [TOUCH_BTN_ATTACK1]    = _BTN_RECT(0, 0),
    [TOUCH_BTN_ATTACK2]    = _BTN_RECT(1, 0),
    [TOUCH_BTN_JUMP]       = _BTN_RECT(1, 1),
    [TOUCH_BTN_SPECIAL]    = _BTN_RECT(4, 0),
    [TOUCH_BTN_START]      = _BTN_RECT(2, 1),
    [TOUCH_BTN_ESC]        = _BTN_RECT(3, 1),
    [TOUCH_BTN_ATTACK3]    = _BTN_RECT(2, 0),
    [TOUCH_BTN_ATTACK4]    = _BTN_RECT(3, 0),
    [TOUCH_BTN_SCREENSHOT] = _BTN_RECT(4, 1)
};

void touch_init(SDL_Renderer *renderer, int screen_w, int screen_h)
{
    g_screen_w = screen_w;
    g_screen_h = screen_h;
    memset(g_pointers, 0, sizeof(g_pointers));
    memset(g_buttons, 0, sizeof(g_buttons));

    for (int i = 0; i < MAX_TOUCH_BUTTONS; i++) {
        g_buttons[i].src_rect = g_default_src_rects[i];
    }

    if (renderer && !g_button_texture) {
        SDL_Surface *surface = pngToSurface((void*)buttonpng);
        if (surface) {
            g_button_texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
            if (g_button_texture) {
                SDL_SetTextureBlendMode(g_button_texture, SDL_BLENDMODE_BLEND);
            }
        }
    }

    touch_update_layout(screen_w, screen_h, 0, 0, 0, 0);
}

void touch_update_layout(int screen_w, int screen_h, float safe_left, float safe_right, float safe_top, float safe_bottom)
{
    g_screen_w = screen_w;
    g_screen_h = screen_h;

    float w = (float)screen_w;
    float h = (float)screen_h;
    float hh = w * 480.0f / 800.0f;
    float ra = 0.15f * g_config.scale;
    float rb = (ra / 1.75f);

    // Safe area offsets
    float margin_x_left = safe_left > 0 ? safe_left + 10.0f : 20.0f;
    float margin_x_right = safe_right > 0 ? safe_right + 10.0f : 20.0f;
    float margin_y_bottom = safe_bottom > 0 ? safe_bottom + 10.0f : 20.0f;

    // D-Pad center
    float dpad_cx = margin_x_left + (ra + rb) * hh;
    float dpad_cy = h - margin_y_bottom - (ra + rb) * hh;
    float dpad_r = rb * hh;

    g_buttons[TOUCH_BTN_UP].x = dpad_cx;
    g_buttons[TOUCH_BTN_UP].y = dpad_cy - ra * hh;
    g_buttons[TOUCH_BTN_UP].radius = dpad_r;

    g_buttons[TOUCH_BTN_RIGHT].x = dpad_cx + ra * hh;
    g_buttons[TOUCH_BTN_RIGHT].y = dpad_cy;
    g_buttons[TOUCH_BTN_RIGHT].radius = dpad_r;

    g_buttons[TOUCH_BTN_DOWN].x = dpad_cx;
    g_buttons[TOUCH_BTN_DOWN].y = dpad_cy + ra * hh;
    g_buttons[TOUCH_BTN_DOWN].radius = dpad_r;

    g_buttons[TOUCH_BTN_LEFT].x = dpad_cx - ra * hh;
    g_buttons[TOUCH_BTN_LEFT].y = dpad_cy;
    g_buttons[TOUCH_BTN_LEFT].radius = dpad_r;

    // Action buttons center (Right side)
    float act_cx = w - margin_x_right - (ra + rb) * hh;
    float act_cy = dpad_cy;

    g_buttons[TOUCH_BTN_SPECIAL].x = act_cx;
    g_buttons[TOUCH_BTN_SPECIAL].y = act_cy - ra * hh;
    g_buttons[TOUCH_BTN_SPECIAL].radius = dpad_r;

    g_buttons[TOUCH_BTN_ATTACK2].x = act_cx + ra * hh;
    g_buttons[TOUCH_BTN_ATTACK2].y = act_cy;
    g_buttons[TOUCH_BTN_ATTACK2].radius = dpad_r;

    g_buttons[TOUCH_BTN_JUMP].x = act_cx;
    g_buttons[TOUCH_BTN_JUMP].y = act_cy + ra * hh;
    g_buttons[TOUCH_BTN_JUMP].radius = dpad_r;

    g_buttons[TOUCH_BTN_ATTACK1].x = act_cx - ra * hh;
    g_buttons[TOUCH_BTN_ATTACK1].y = act_cy;
    g_buttons[TOUCH_BTN_ATTACK1].radius = dpad_r;

    // Start & Pause/Esc at upper corners
    g_buttons[TOUCH_BTN_START].x = margin_x_left + dpad_r;
    g_buttons[TOUCH_BTN_START].y = safe_top + dpad_r + 10.0f;
    g_buttons[TOUCH_BTN_START].radius = dpad_r * 0.85f;

    g_buttons[TOUCH_BTN_ESC].x = w - margin_x_right - dpad_r;
    g_buttons[TOUCH_BTN_ESC].y = safe_top + dpad_r + 10.0f;
    g_buttons[TOUCH_BTN_ESC].radius = dpad_r * 0.85f;

    // Secondary attacks at bottom center
    g_buttons[TOUCH_BTN_ATTACK3].x = w * 0.5f - ra * hh;
    g_buttons[TOUCH_BTN_ATTACK3].y = h - margin_y_bottom - dpad_r;
    g_buttons[TOUCH_BTN_ATTACK3].radius = dpad_r * 0.85f;

    g_buttons[TOUCH_BTN_ATTACK4].x = w * 0.5f + ra * hh;
    g_buttons[TOUCH_BTN_ATTACK4].y = h - margin_y_bottom - dpad_r;
    g_buttons[TOUCH_BTN_ATTACK4].radius = dpad_r * 0.85f;

    // Destination rects
    for (int i = 0; i < MAX_TOUCH_BUTTONS; i++) {
        g_buttons[i].dst_rect.x = (int)(g_buttons[i].x - g_buttons[i].radius);
        g_buttons[i].dst_rect.y = (int)(g_buttons[i].y - g_buttons[i].radius);
        g_buttons[i].dst_rect.w = (int)(g_buttons[i].radius * 2.0f);
        g_buttons[i].dst_rect.h = (int)(g_buttons[i].radius * 2.0f);
    }
}

void touch_handle_event(const SDL_Event *event)
{
    if (!event) return;

    if (event->type == SDL_FINGERDOWN || event->type == SDL_FINGERMOTION) {
        float px = event->tfinger.x * (float)g_screen_w;
        float py = event->tfinger.y * (float)g_screen_h;

        // Register / update pointer
        int slot = -1;
        for (int i = 0; i < MAX_TOUCH_POINTERS; i++) {
            if (g_pointers[i].down && g_pointers[i].id == event->tfinger.fingerId) {
                slot = i;
                break;
            }
        }
        if (slot < 0) {
            for (int i = 0; i < MAX_TOUCH_POINTERS; i++) {
                if (!g_pointers[i].down) {
                    slot = i;
                    break;
                }
            }
        }
        if (slot >= 0) {
            g_pointers[slot].id = event->tfinger.fingerId;
            g_pointers[slot].x = px;
            g_pointers[slot].y = py;
            g_pointers[slot].down = 1;
        }
    } else if (event->type == SDL_FINGERUP) {
        for (int i = 0; i < MAX_TOUCH_POINTERS; i++) {
            if (g_pointers[i].down && g_pointers[i].id == event->tfinger.fingerId) {
                g_pointers[i].down = 0;
                break;
            }
        }
    }

    // Recalculate button states from active pointers
    for (int b = 0; b < MAX_TOUCH_BUTTONS; b++) {
        g_buttons[b].state = 0;
    }

    for (int p = 0; p < MAX_TOUCH_POINTERS; p++) {
        if (!g_pointers[p].down) continue;
        float px = g_pointers[p].x;
        float py = g_pointers[p].y;

        for (int b = 0; b < MAX_TOUCH_BUTTONS; b++) {
            float dx = px - g_buttons[b].x;
            float dy = py - g_buttons[b].y;
            float dist_sq = dx * dx + dy * dy;
            float rad = g_buttons[b].radius * 1.25f; // Slight padding for thumb
            if (dist_sq <= rad * rad) {
                g_buttons[b].state = 1;
            }
        }
    }
}

void touch_render(SDL_Renderer *renderer)
{
    if (!renderer || !g_button_texture || !g_config.visible) return;

    for (int i = 0; i < MAX_TOUCH_BUTTONS; i++) {
        Uint8 alpha = g_buttons[i].state ? (Uint8)g_config.alpha_pressed : (Uint8)g_config.alpha_normal;
        SDL_SetTextureAlphaMod(g_button_texture, alpha);
        SDL_RenderCopy(renderer, g_button_texture, &g_buttons[i].src_rect, &g_buttons[i].dst_rect);
    }
}

void touch_apply_inputs(Uint8 *keystate)
{
    if (!keystate || !g_config.visible) return;

    if (g_buttons[TOUCH_BTN_UP].state)         keystate[SDL_SCANCODE_UP] = 1;
    if (g_buttons[TOUCH_BTN_DOWN].state)       keystate[SDL_SCANCODE_DOWN] = 1;
    if (g_buttons[TOUCH_BTN_LEFT].state)       keystate[SDL_SCANCODE_LEFT] = 1;
    if (g_buttons[TOUCH_BTN_RIGHT].state)      keystate[SDL_SCANCODE_RIGHT] = 1;
    if (g_buttons[TOUCH_BTN_ATTACK1].state)    keystate[SDL_SCANCODE_A] = 1;
    if (g_buttons[TOUCH_BTN_ATTACK2].state)    keystate[SDL_SCANCODE_S] = 1;
    if (g_buttons[TOUCH_BTN_JUMP].state)       keystate[SDL_SCANCODE_D] = 1;
    if (g_buttons[TOUCH_BTN_SPECIAL].state)    keystate[SDL_SCANCODE_F] = 1;
    if (g_buttons[TOUCH_BTN_START].state)      keystate[SDL_SCANCODE_RETURN] = 1;
    if (g_buttons[TOUCH_BTN_ESC].state)        keystate[SDL_SCANCODE_ESCAPE] = 1;
}

void touch_set_visible(int visible)
{
    g_config.visible = visible ? 1 : 0;
}

void touch_set_scale(float scale)
{
    if (scale >= 0.5f && scale <= 2.5f) {
        g_config.scale = scale;
    }
}

void touch_set_opacity(int normal, int pressed)
{
    g_config.alpha_normal = normal;
    g_config.alpha_pressed = pressed;
}
