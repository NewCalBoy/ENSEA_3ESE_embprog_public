/*
 * screen_switch_entry.h
 *
 *  Created on: Sep 29, 2026
 *      Author: laurentf
 */

#ifndef TD4_COMPLETE_APP_SCREEN_SWITCH_ENTRY_H_
#define TD4_COMPLETE_APP_SCREEN_SWITCH_ENTRY_H_

#include "menu_entry.h"

/* CONCRETE ENTRY: navigates to another menu_t (used both to open the HSV
 * page and to come back from it) */
typedef struct {
	menu_entry_t super; /* inherited menu_entry_t */

	menu_t *target;
} screen_switch_entry_t;

void screen_switch_entry_init(screen_switch_entry_t *e, const char *label, menu_t *target);

void screen_switch_entry_draw(const screen_switch_entry_t *e, bsp_t *bsp, int y, bool selected);

#endif /* TD4_COMPLETE_APP_SCREEN_SWITCH_ENTRY_H_ */
