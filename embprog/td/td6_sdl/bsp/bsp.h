#ifndef BSP_H
#define BSP_H

#include <stdbool.h>
#include <stdint.h>

#include "events.h"

/*
 * BSP vue par l'app -- meme struct (memes noms de champs) que la BSP
 * materielle de td5 (td/td5_complete/bsp/bsp.h) : app/ (repris tel quel,
 * voir CMakeLists.txt) n'a aucune idee qu'elle tourne sur un simulateur
 * plutot que sur la carte.
 *
 * REGLE : c'est bsp.c (et les modules qu'il assemble) le seul a inclure
 * SDL3 et a savoir que l'on tourne dans un simulateur -- pas ce header.
 */
typedef struct bsp_struct bsp_t;

struct bsp_struct {
    void *ctx;

    void (*led_set)(bsp_t *bsp, uint8_t led, bool on);
    void (*neopixel_set_hsv)(bsp_t *bsp, int hue, int sat, int val);
    bool (*sw1_pressed)(bsp_t *bsp);
    bool (*sw2_pressed)(bsp_t *bsp);
    uint32_t (*get_tick_ms)(bsp_t *bsp);

    int32_t (*encoder_get_delta)(bsp_t *bsp);
    bool (*enc_pressed)(bsp_t *bsp);

    void (*oled_clear)(bsp_t *bsp);
    void (*oled_fill_rect)(bsp_t *bsp, int x, int y, int w, int h, bool on);
    void (*oled_draw_string)(bsp_t *bsp, int x, int y, const char *str, bool on);
    void (*oled_show)(bsp_t *bsp);
};

/*
 * Cycle de vie du simulateur (fenetre SDL a ouvrir/fermer, boucle
 * d'evenements a depiler) : n'existe pas cote materiel, reserve a main.c.
 * `events` est stocke (comme bsp_ctx_t.events cote materiel) : c'est
 * bsp_process qui y pousse les evenements, pas l'appelant -- meme
 * signature que bsp_init(bsp_t *, event_queue_t *) cote td5_complete.
 */
bool bsp_init(bsp_t *bsp, event_queue_t *events);
void bsp_deinit(bsp_t *bsp);
bool bsp_process(bsp_t *bsp);

#endif /* BSP_H */
