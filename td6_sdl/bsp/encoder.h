#ifndef BSP_ENCODER_H
#define BSP_ENCODER_H

#include <stdbool.h>
#include <stdint.h>

#include <SDL3/SDL.h>

/* Module simulateur de l'encodeur incremental (+ / - / push) : touches
 * HAUT/BAS/ENTREE ou clic sur les rectangles a l'ecran. Le bouton-poussoir
 * est necessaire ici pour naviguer dans le menu . */

typedef struct {
    int32_t delta;
    bool button_edge;
    bool plus_held;
    bool minus_held;
    bool push_held;
} bsp_encoder_t;

void bsp_encoder_init(bsp_encoder_t *enc);
void bsp_encoder_on_key(bsp_encoder_t *enc, SDL_Keycode key, bool down, bool repeat);
void bsp_encoder_on_click(bsp_encoder_t *enc, float x, float y, bool down);
int32_t bsp_encoder_take_delta(bsp_encoder_t *enc);
bool bsp_encoder_take_button(bsp_encoder_t *enc);
void bsp_encoder_render(const bsp_encoder_t *enc, SDL_Renderer *renderer);

#endif /* BSP_ENCODER_H */
