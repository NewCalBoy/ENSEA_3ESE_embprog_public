/*
 * led_toggle_entry.h
 *
 *  Created on: Sep 29, 2026
 *      Author: laurentf
 */

#ifndef TD4_COMPLETE_APP_LED_TOGGLE_ENTRY_H_
#define TD4_COMPLETE_APP_LED_TOGGLE_ENTRY_H_

#include "menu_entry.h"

/* CONCRETE ENTRY: toggles one LED on/off */
typedef struct {
	menu_entry_t super; /* inherited menu_entry_t */

	bool *led_on;
	uint8_t led_index;
} led_toggle_entry_t;

void led_toggle_entry_init(led_toggle_entry_t *e, const char *label, bool *led_on, uint8_t led_index);

void led_toggle_entry_toggle(led_toggle_entry_t *e, bsp_t *bsp);
void led_toggle_entry_draw(const led_toggle_entry_t *e, bsp_t *bsp, int y, bool selected);

#endif /* TD4_COMPLETE_APP_LED_TOGGLE_ENTRY_H_ */
