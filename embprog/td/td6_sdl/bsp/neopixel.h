#ifndef BSP_NEOPIXEL_H
#define BSP_NEOPIXEL_H

#include <SDL3/SDL.h>

/* Module simulateur du neopixel WS2812B, pilote en HSV. */

typedef struct {
    int hue; /* 0..359 */
    int sat; /* 0..255 */
    int val; /* 0..255 */
} bsp_neopixel_t;

void bsp_neopixel_init(bsp_neopixel_t *np);
void bsp_neopixel_write(bsp_neopixel_t *np, int hue, int sat, int val);
void bsp_neopixel_render(const bsp_neopixel_t *np, SDL_Renderer *renderer);

#endif /* BSP_NEOPIXEL_H */
