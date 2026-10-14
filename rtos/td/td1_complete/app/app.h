/*
 * app.h
 *
 *  Created on: Sep 27, 2026
 *      Author: laurentf
 */

#ifndef TD3_COMPLETE_APP_APP_H_
#define TD3_COMPLETE_APP_APP_H_

#include <stdbool.h>

#include "events.h"

/* Fonctions à appeler dans le main */
void app_init(void);
void app_process(void);

/* Seul point d'entrée vers la file d'événements (qui appartient à l'app).
 * Renvoie false si la file est pleine. */
bool app_post_event(app_event_t evt);

#endif /* TD3_COMPLETE_APP_APP_H_ */
