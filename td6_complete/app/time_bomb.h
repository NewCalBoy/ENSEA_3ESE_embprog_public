#ifndef APP_TIME_BOMB_H_
#define APP_TIME_BOMB_H_

#include <stdbool.h>
#include <stdint.h>

#include "bsp.h"
#include "events.h"
#include "timers.h"

/*
 * TIME BOMB -- l'exemple classique de Miro Samek, en 3 etats :
 *
 *   IDLE --(SW1 click)--> ARMED --(timer, blink_count < 3)--> ARMED (boucle)
 *                                --(timer, blink_count >= 3)--> EXPLODED
 *
 * IDLE et EXPLODED acceptent SW2 (retour au menu) ; ARMED non -- on ne
 * peut plus annuler une fois la bombe lancee. Ce n'est pas un menu_t :
 * pas de curseur, pas d'entrees, juste son propre ecran (voir app_mode_t
 * et time_bomb_launch_entry).
 */
typedef enum {
    BOMB_IDLE,
    BOMB_ARMED,
    BOMB_EXPLODED,
} bomb_state_t;

typedef struct {
    bomb_state_t state;
    int blink_count;

    /* Meme champ partage que led_toggle_entry_t : la bombe pilote la
     * meme LED que la page LEDS, donc le menu reste a jour quand on y
     * revient. */
    bool *led_on;
    uint8_t led_index;

    app_timers_t *timers;
    int timer_id;
} time_bomb_t;

void time_bomb_init(time_bomb_t *tb, bool *led_on, uint8_t led_index, app_timers_t *timers, int timer_id);

/* (Re)entre sur l'ecran : repart toujours de BOMB_IDLE, LED eteinte,
 * timer desarme. */
void time_bomb_enter(time_bomb_t *tb, bsp_t *bsp);

/* Renvoie true si cet evenement doit faire quitter l'ecran (retour au
 * menu principal) -- a l'appelant (app_process) de rebasculer le mode. */
bool time_bomb_handle_event(time_bomb_t *tb, const app_event_t *evt, bsp_t *bsp, uint32_t now_ms);

void time_bomb_draw(const time_bomb_t *tb, bsp_t *bsp);

#endif /* APP_TIME_BOMB_H_ */
