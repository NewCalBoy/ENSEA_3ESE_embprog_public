/*
 * led_toggle_entry.c
 *
 *  Created on: Sep 29, 2026
 *      Author: laurentf
 */

#include "led_toggle_entry.h"

#include <stdio.h>

void led_toggle_entry_init(led_toggle_entry_t *e, const char *label, bool *led_on, uint8_t led_index) {
	e->super.kind = MENU_ENTRY_LED_TOGGLE;
	e->super.label = label;
	e->led_on = led_on;
	e->led_index = led_index;
}

void led_toggle_entry_toggle(led_toggle_entry_t *e) {
	*e->led_on = !*e->led_on;
	bsp_led_set(e->led_index, *e->led_on);
}

void led_toggle_entry_draw(const led_toggle_entry_t *e, int y, bool selected) {
	char line[20];
	snprintf(line, sizeof(line), "%s: %s", e->super.label, *e->led_on ? "ON" : "OFF");
	menu_entry_draw_row(y, selected, false, line);
}
