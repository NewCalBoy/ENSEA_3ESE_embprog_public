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

/* Fonction à appeler dans le main, avant vTaskStartScheduler().
 * L'app est un objet actif : app_init crée sa file d'événements et sa
 * propre tâche. La priorité est choisie par le main : c'est une décision
 * système (qui passe avant qui), pas une décision de l'app. */
void app_init(unsigned int task_priority);

/* Interface publique de l'objet actif : seul point d'entrée vers sa file
 * d'événements (privée). Ne bloque jamais (timeout 0) : renvoie false si
 * la file est pleine.
 * - app_post_event          : depuis une tâche
 * - app_post_event_from_isr : depuis une interruption */
bool app_post_event(app_event_t evt);
bool app_post_event_from_isr(app_event_t evt);

#endif /* TD3_COMPLETE_APP_APP_H_ */
