#ifndef BSP_DISPLAY_H
#define BSP_DISPLAY_H

#include <stdbool.h>

#include <SDL3/SDL.h>

#include "bsp_defs.h"

/* Module simulateur de l'ecran OLED SSD1306 128x64 1bpp : framebuffer
 * logiciel + police fixe 5x7, affiches agrandis dans la fenetre. */

typedef struct {
    bool pixels[BSP_OLED_HEIGHT][BSP_OLED_WIDTH];
} bsp_display_t;

void bsp_display_init(bsp_display_t *disp);
void bsp_display_clear(bsp_display_t *disp);
void bsp_display_fill(bsp_display_t *disp, int x, int y, int w, int h, bool on);
void bsp_display_text(bsp_display_t *disp, int x, int y, const char *str, bool on);
void bsp_display_render(const bsp_display_t *disp, SDL_Renderer *renderer);

#endif /* BSP_DISPLAY_H */
