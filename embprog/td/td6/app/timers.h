#ifndef APP_TIMERS_H
#define APP_TIMERS_H

#include <stdbool.h>
#include <stdint.h>

#include "events.h"

/*
 * Service de timers logiciels, entierement cote app : la BSP n'expose
 * qu'une horloge (bsp_t.get_tick_ms), tout le reste est generique et n'a
 * aucune dependance HAL.
 *
 * Chaque timer est arme avec une echeance (un instant, pas une duree) --
 * comme un timer Linux, pas un intervalle qui se repete tout seul. Il
 * tire une fois puis se desarme. Pour du periodique, c'est a l'appelant
 * de rearmer en reponse a EVT_TIMER, avec la nouvelle echeance de son
 * choix (typiquement l'ancienne echeance + une periode).
 */

#define APP_TIMER_COUNT 4

typedef struct {
    bool active;
    uint32_t deadline_ms;
} app_timer_t;

typedef struct {
    app_timer_t slots[APP_TIMER_COUNT];
} app_timers_t;

void app_timers_init(app_timers_t *timers);

/* Arme (ou rearme) le timer `id` pour qu'il tire a `deadline_ms`. Ne fait
 * rien si `id` est hors bornes. */
void app_timer_start(app_timers_t *timers, int id, uint32_t deadline_ms);

/* Desarme le timer `id`. Ne fait rien si `id` est hors bornes. */
void app_timer_stop(app_timers_t *timers, int id);

/* A appeler une fois par tour de superloop (app_process) : pousse un
 * EVT_TIMER{.data = id} pour chaque timer actif dont l'echeance est
 * depassee, puis le desarme (one-shot). */
void app_timers_process(app_timers_t *timers, uint32_t now_ms, event_queue_t *events);

#endif /* APP_TIMERS_H */
