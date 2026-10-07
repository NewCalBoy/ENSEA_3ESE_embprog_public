#ifndef APP_TIME_BOMB_H_
#define APP_TIME_BOMB_H_

#include <stdbool.h>
#include <stdint.h>

#include "bsp.h"
#include "events.h"
#include "timers.h"

/*
 * TIME BOMB -- point de depart pour la seance 6. Pour l'instant ce n'est
 * pas une machine a etats : l'ecran fait juste clignoter LED1 en continu
 * des qu'on y entre, histoire de montrer comment armer un timer et
 * reagir a son EVT_TIMER (voir timers.h). Les etats (attente / armee /
 * explosee), le compteur de clignotements et la garde qui declenche
 * l'explosion sont a construire en seance.
 */
typedef struct {
    bool *led_on;
    uint8_t led_index;

    app_timers_t *timers;
    int timer_id;
} time_bomb_t;

void time_bomb_init(time_bomb_t *tb, bool *led_on, uint8_t led_index, app_timers_t *timers, int timer_id);

/* (Re)entre sur l'ecran : LED eteinte, timer de clignotement (re)arme. */
void time_bomb_enter(time_bomb_t *tb, bsp_t *bsp);

/* Renvoie true si cet evenement doit faire quitter l'ecran (retour au
 * menu principal) -- a l'appelant (app_process) de rebasculer le mode. */
bool time_bomb_handle_event(time_bomb_t *tb, const app_event_t *evt, bsp_t *bsp, uint32_t now_ms);

void time_bomb_draw(const time_bomb_t *tb, bsp_t *bsp);

#endif /* APP_TIME_BOMB_H_ */
