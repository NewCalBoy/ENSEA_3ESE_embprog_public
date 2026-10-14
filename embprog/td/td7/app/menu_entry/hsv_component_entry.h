/*
 * hsv_component_entry.h
 *
 *  Created on: Sep 29, 2026
 *      Author: laurentf
 */

#ifndef TD4_COMPLETE_APP_HSV_COMPONENT_ENTRY_H_
#define TD4_COMPLETE_APP_HSV_COMPONENT_ENTRY_H_

#include "menu_entry.h"

/* CONCRETE ENTRY: edits one bounded int value (used for HUE/SAT/VAL) */
typedef struct {
	menu_entry_t super; /* inherited menu_entry_t */

	int *value;
	int min;
	int max;
	int step;
} hsv_component_entry_t;

void hsv_component_entry_init(hsv_component_entry_t *e, const char *label, int *value, int min, int max, int step);

void hsv_component_entry_adjust(hsv_component_entry_t *e, int32_t delta);
void hsv_component_entry_draw(const hsv_component_entry_t *e, bsp_t *bsp, int y, bool selected, bool editing);

#endif /* TD4_COMPLETE_APP_HSV_COMPONENT_ENTRY_H_ */
