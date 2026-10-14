#include "neopixel.h"

#include <stdint.h>

static const SDL_FRect RECT_NEOPIXEL = {160, 12, 40, 40};

void bsp_neopixel_init(bsp_neopixel_t *np) {
    np->hue = 0;
    np->sat = 0;
    np->val = 0;
}

void bsp_neopixel_write(bsp_neopixel_t *np, int hue, int sat, int val) {
    np->hue = hue;
    np->sat = sat;
    np->val = val;
}

/* Conversion HSV (0..359, 0..255, 0..255) vers RGB 8 bits, comme le
 * ferait le driver avant d'ecrire les octets WS2812B. */
static void hsv_to_rgb(int hue, int sat, int val, uint8_t *r, uint8_t *g, uint8_t *b) {
    float h = (float)((hue % 360 + 360) % 360) / 60.0f;
    float s = (float)sat / 255.0f;
    float v = (float)val / 255.0f;

    int i = (int)h;
    float f = h - (float)i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));
    float rf, gf, bf;

    switch (i % 6) {
        case 0: rf = v; gf = t; bf = p; break;
        case 1: rf = q; gf = v; bf = p; break;
        case 2: rf = p; gf = v; bf = t; break;
        case 3: rf = p; gf = q; bf = v; break;
        case 4: rf = t; gf = p; bf = v; break;
        default: rf = v; gf = p; bf = q; break;
    }

    *r = (uint8_t)(rf * 255.0f);
    *g = (uint8_t)(gf * 255.0f);
    *b = (uint8_t)(bf * 255.0f);
}

/*
 * Boost d'affichage, simulateur uniquement : un neopixel physique a une
 * valeur (V) basse (typiquement ~20/255 pour ne pas eblouir) reste tout a
 * fait visible a l'oeil nu, mais rendue telle quelle en pixel d'ecran elle
 * se fond dans l'interface sombre. On corrige avec une gamma (l'oeil
 * percoit la luminosite de facon non lineaire) uniquement pour l'affichage
 * -- la valeur HSV recue par la "BSP" n'est pas modifiee, et reste celle
 * affichee sur l'ecran OLED simule (page HSV) : les deux restent coherents.
 */
static uint8_t screen_gamma(uint8_t channel) {
    float linear = (float)channel / 255.0f;
    return (uint8_t)(SDL_powf(linear, 0.45f) * 255.0f);
}

void bsp_neopixel_render(const bsp_neopixel_t *np, SDL_Renderer *renderer) {
    uint8_t r, g, b;
    hsv_to_rgb(np->hue, np->sat, np->val, &r, &g, &b);
    r = screen_gamma(r);
    g = screen_gamma(g);
    b = screen_gamma(b);

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderFillRect(renderer, &RECT_NEOPIXEL);
    SDL_SetRenderDrawColor(renderer, 150, 150, 155, 255);
    SDL_RenderRect(renderer, &RECT_NEOPIXEL);

    SDL_SetRenderDrawColor(renderer, 200, 200, 205, 255);
    SDL_RenderDebugText(renderer, RECT_NEOPIXEL.x - 6, RECT_NEOPIXEL.y + 44, "NEOPIXEL");
}
