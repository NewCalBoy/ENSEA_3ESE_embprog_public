/*
 * stats.h
 *
 * Objet "stats" : une tâche de priorité basse qui affiche des statistiques
 * sur la console (UART2) toutes les secondes.
 */

#ifndef STATS_STATS_H_
#define STATS_STATS_H_

/* À appeler dans le main, avant vTaskStartScheduler(). Comme la BSP et
 * l'app, l'objet crée sa propre tâche ; le main choisit la priorité. */
void stats_init(unsigned int task_priority);

#endif /* STATS_STATS_H_ */
