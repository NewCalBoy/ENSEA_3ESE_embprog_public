/*
 * bsp.h
 *
 *  Created on: Sep 27, 2026
 *      Author: laurentf
 */

#ifndef TD3_COMPLETE_BSP_BSP_H_
#define TD3_COMPLETE_BSP_BSP_H_

#include <stdint.h>
#include <stdbool.h>
/* Fonction à appeler dans le main, avant vTaskStartScheduler().
 * La BSP crée sa propre tâche (scrutation périodique des entrées).
 * La priorité est choisie par le main : c'est une décision système. */
void bsp_init(unsigned int task_priority);

/* Ecran : la BSP crée une tâche d'affichage. bsp_oled_show() ne fait plus
 * le transfert : elle demande un rafraîchissement et revient tout de suite.
 * À appeler avant app_init() (l'app dessine dès son init). */
void bsp_init_display(unsigned int task_priority);

void bsp_led_set(uint8_t led, bool on);
void bsp_neopixel_set_hsv(int hue, int sat, int val);
uint32_t bsp_get_tick_ms(void);

void bsp_oled_clear(void);
void bsp_oled_fill_rect(int x, int y, int w, int h, bool on);
void bsp_oled_draw_string(int x, int y, const char *str, bool on);
void bsp_oled_show(void);

#endif /* TD3_COMPLETE_BSP_BSP_H_ */
