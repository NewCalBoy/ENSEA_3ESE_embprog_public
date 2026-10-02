/*
 * hsv_component_entry.c
 *
 *  Created on: Sep 29, 2026
 *      Author: laurentf
 */

#include "hsv_component_entry.h"

#include <stdio.h>

void hsv_component_entry_init(hsv_component_entry_t *e, const char *label, int *value, int min, int max, int step) {
	e->super.kind = MENU_ENTRY_HSV_COMPONENT;
	e->super.label = label;
	e->value = value;
	e->min = min;
	e->max = max;
	e->step = step;
}

void hsv_component_entry_adjust(hsv_component_entry_t *e, int32_t delta) {
	int value = *e->value + (int)delta * e->step;
	if (value < e->min) {
		value = e->min;
	}
	if (value > e->max) {
		value = e->max;
	}
	*e->value = value;
}

void hsv_component_entry_draw(const hsv_component_entry_t *e, bsp_t *bsp, int y, bool selected, bool editing) {
	char line[20];
	snprintf(line, sizeof(line), "%s: %d", e->super.label, *e->value);
	menu_entry_draw_row(bsp, y, selected, editing, line);
}
