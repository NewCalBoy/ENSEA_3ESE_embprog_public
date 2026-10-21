/*
 * stats.c
 *
 * Objet "stats" : affiche des statistiques toutes les secondes.
 *
 * Pourquoi une tâche dédiée, et pas un handler de l'app ?
 * - printf sur l'UART, en polling : des dizaines de ms. Dans un handler de
 *   l'app, ça casserait le run-to-completion.
 * - vTaskList() et vTaskGetRunTimeStats() suspendent le scheduler pendant
 *   leur exécution : ce sont des outils de debug, à garder en priorité basse.
 */

#include "stats.h"

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#define STATS_STACK_DEPTH 256	/* en mots de 32 bits -- TODO : suffisant pour vTaskList ? */
#define STATS_PERIOD_MS   1000

static void stats_task(void *unused) {
	(void)unused;

	for (;;) {
		vTaskDelay(pdMS_TO_TICKS(STATS_PERIOD_MS));

		/* Uptime calculé depuis le tick : pas de dérive, même si la tâche
		 * est retardée par des tâches plus prioritaires */
		unsigned long uptime_s = (unsigned long)(xTaskGetTickCount() / configTICK_RATE_HZ);
		printf("\r\n--- uptime: %lu s ---\r\n", uptime_s);

		/* TODO 1 -- Le tas : taille, libre, minimum jamais atteint
		 *   configTOTAL_HEAP_SIZE, xPortGetFreeHeapSize(), xPortGetMinimumEverFreeHeapSize() */

		/* TODO 2 -- Les piles : high water mark de chaque tâche
		 *   uxTaskGetStackHighWaterMark(), puis vTaskList() */

		/* TODO 3 -- Le temps CPU
		 *   vTaskGetRunTimeStats() */
	}
}

void stats_init(unsigned int task_priority) {
	BaseType_t ok = xTaskCreate(stats_task, "STATS", STATS_STACK_DEPTH,
			NULL, task_priority, NULL);
	configASSERT(ok == pdPASS);
}

/* -------------------------------------------------------------------------
 * Compteur de temps pour les statistiques d'exécution (fourni).
 * Actif une fois GENERATE_RUN_TIME_STATS activé dans CubeMX, et TIM7
 * configuré à 10 kHz (prescaler), ARR = 0xFFFF.
 * ------------------------------------------------------------------------- */
#if (configGENERATE_RUN_TIME_STATS == 1)

#include "tim.h"

void configureTimerForRunTimeStats(void) {
	HAL_TIM_Base_Start(&htim7);
}

/* TIM7 compte sur 16 bits, FreeRTOS veut 32 bits. La différence sur 16 bits
 * reste correcte quand le compteur déborde, à condition d'être appelée au
 * moins une fois par débordement (6,5 s à 10 kHz) : FreeRTOS l'appelle à
 * chaque changement de contexte, et la BSP se réveille toutes les 5 ms. */
unsigned long getRunTimeCounterValue(void) {
	static uint16_t prev = 0;
	static uint32_t total = 0;

	uint16_t cnt = __HAL_TIM_GET_COUNTER(&htim7);
	total += (uint16_t)(cnt - prev);
	prev = cnt;
	return total;
}

#endif /* configGENERATE_RUN_TIME_STATS */
