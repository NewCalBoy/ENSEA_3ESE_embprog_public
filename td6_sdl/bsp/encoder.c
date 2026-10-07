#include "encoder.h"

static const SDL_FRect RECT_PLUS  = {580, 20, 100, 60};
static const SDL_FRect RECT_PUSH  = {580, 90, 100, 60};
static const SDL_FRect RECT_MINUS = {580, 160, 100, 60};

static void draw_button(SDL_Renderer *renderer, const SDL_FRect *rect, bool held, const char *label) {
    if (held) {
        SDL_SetRenderDrawColor(renderer, 90, 130, 180, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 55, 60, 70, 255);
    }
    SDL_RenderFillRect(renderer, rect);
    SDL_SetRenderDrawColor(renderer, 170, 175, 185, 255);
    SDL_RenderRect(renderer, rect);

    float tx = rect->x + rect->w / 2.0f - (float)SDL_strlen(label) * 4.0f;
    float ty = rect->y + rect->h / 2.0f - 4.0f;
    SDL_SetRenderDrawColor(renderer, 235, 235, 240, 255);
    SDL_RenderDebugText(renderer, tx, ty, label);
}

void bsp_encoder_init(bsp_encoder_t *enc) {
    enc->delta = 0;
    enc->button_edge = false;
    enc->plus_held = false;
    enc->minus_held = false;
    enc->push_held = false;
}

void bsp_encoder_on_key(bsp_encoder_t *enc, SDL_Keycode key, bool down, bool repeat) {
    switch (key) {
        case SDLK_DOWN:
            enc->plus_held = down;
            if (down) {
                enc->delta++;
            }
            break;
        case SDLK_UP:
            enc->minus_held = down;
            if (down) {
                enc->delta--;
            }
            break;
        case SDLK_RETURN:
        case SDLK_KP_ENTER:
            enc->push_held = down;
            if (down && !repeat) {
                enc->button_edge = true;
            }
            break;
        default:
            break;
    }
}

void bsp_encoder_on_click(bsp_encoder_t *enc, float x, float y, bool down) {
    SDL_FPoint p = {x, y};

    bool in_plus = SDL_PointInRectFloat(&p, &RECT_PLUS);
    bool in_minus = SDL_PointInRectFloat(&p, &RECT_MINUS);
    bool in_push = SDL_PointInRectFloat(&p, &RECT_PUSH);

    if (down) {
        if (in_plus) {
            enc->plus_held = true;
            enc->delta++;
        }
        if (in_minus) {
            enc->minus_held = true;
            enc->delta--;
        }
        if (in_push) {
            enc->push_held = true;
            enc->button_edge = true;
        }
    } else {
        enc->plus_held = false;
        enc->minus_held = false;
        enc->push_held = false;
    }
}

int32_t bsp_encoder_take_delta(bsp_encoder_t *enc) {
    int32_t delta = enc->delta;
    enc->delta = 0;
    return delta;
}

bool bsp_encoder_take_button(bsp_encoder_t *enc) {
    bool edge = enc->button_edge;
    enc->button_edge = false;
    return edge;
}

void bsp_encoder_render(const bsp_encoder_t *enc, SDL_Renderer *renderer) {
    draw_button(renderer, &RECT_PLUS, enc->plus_held, "+");
    draw_button(renderer, &RECT_PUSH, enc->push_held, "PUSH");
    draw_button(renderer, &RECT_MINUS, enc->minus_held, "-");
}
