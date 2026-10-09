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

#include "events.h"

/* BSP vue par l'app */
typedef struct bsp_struct bsp_t;

struct bsp_struct {
	void * ctx;

	void (*led_set)(bsp_t * bsp, uint8_t led, bool on);
	void (*neopixel_set_hsv)(bsp_t * bsp, int hue, int sat, int val);
	uint32_t (*get_tick_ms)(bsp_t * bsp);

	void (*oled_clear)(bsp_t * bsp);
	void (*oled_fill_rect)(bsp_t * bsp, int x, int y, int w, int h, bool on);
	void (*oled_draw_string)(bsp_t * bsp, int x, int y, const char *str, bool on);
	void (*oled_show)(bsp_t * bsp);
};

/* Fonctions à appeler dans le main */
void bsp_init(bsp_t * bsp, event_queue_t * events);
void bsp_process(bsp_t * bsp);

#endif /* TD3_COMPLETE_BSP_BSP_H_ */
