/*
 * screen_switch_entry.c
 *
 *  Created on: Sep 29, 2026
 *      Author: laurentf
 */

#include "screen_switch_entry.h"

void screen_switch_entry_init(screen_switch_entry_t *e, const char *label, menu_t *target) {
	e->super.kind = MENU_ENTRY_SCREEN_SWITCH;
	e->super.label = label;
	e->target = target;
}

void screen_switch_entry_draw(const screen_switch_entry_t *e, bsp_t *bsp, int y, bool selected) {
	menu_entry_draw_row(bsp, y, selected, false, e->super.label);
}
