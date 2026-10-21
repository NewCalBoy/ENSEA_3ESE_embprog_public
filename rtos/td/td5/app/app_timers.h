#ifndef APP_TIMERS_H
#define APP_TIMERS_H

#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "timers.h"		/* timers FreeRTOS (d'où le renommage de notre fichier en app_timers) */

#include "events.h"

/*
 * Service de timers de l'app, réimplémenté au-dessus des timers logiciels
 * FreeRTOS. L'interface ne change pas (time_bomb et runner ne sont pas
 * modifiés) : seule l'implémentation change.
 *
 * Chaque timer est armé avec une échéance (un instant, pas une durée) --
 * comme un timer Linux, pas un intervalle qui se répète tout seul. Il
 * tire une fois puis se désarme (timer FreeRTOS "one-shot"). Pour du
 * périodique, c'est à l'appelant de réarmer en réponse à EVT_TIMER, avec
 * la nouvelle échéance de son choix (typiquement l'ancienne échéance +
 * une période).
 *
 * À l'échéance, le callback s'exécute dans la tâche timer de FreeRTOS
 * (daemon), pas dans l'app : il se contente de poster EVT_TIMER{.data = id}
 * dans la file de l'app. Plus besoin de scruter les timers : la tâche de
 * l'app peut bloquer indéfiniment sur sa file.
 */

#define APP_TIMER_COUNT 4

typedef struct {
    TimerHandle_t handles[APP_TIMER_COUNT];
} app_timers_t;

/* Crée les timers FreeRTOS (dans le tas). À appeler avant d'armer un timer. */
void app_timers_init(app_timers_t *timers);

/* Arme (ou réarme) le timer `id` pour qu'il tire à `deadline_ms` (horloge
 * bsp_get_tick_ms). Ne fait rien si `id` est hors bornes. */
void app_timer_start(app_timers_t *timers, int id, uint32_t deadline_ms);

/* Désarme le timer `id`. Ne fait rien si `id` est hors bornes.
 * Attention : un EVT_TIMER déjà posté dans la file de l'app peut encore
 * arriver après l'arrêt. */
void app_timer_stop(app_timers_t *timers, int id);

#endif /* APP_TIMERS_H */
