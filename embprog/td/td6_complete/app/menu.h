/*
 * menu.h
 *
 *  Created on: Sep 29, 2026
 *      Author: laurentf
 */

#ifndef TD4_COMPLETE_APP_MENU_H_
#define TD4_COMPLETE_APP_MENU_H_

#include <stdint.h>
#include <stdbool.h>

#include "bsp.h"
#include "menu_entry.h"

/* MENU LOGIC -- a list of entries; talks to them only through the
 * menu_entry_* polymorphic operations above, never through a concrete type. */

#define MAX_ENTRIES 4

struct menu {
	menu_entry_t *entries[MAX_ENTRIES];
	int count;
	int cursor;
	bool editing;
};

void menu_init(menu_t *m);
void menu_add_entry(menu_t *m, menu_entry_t *entry);
void menu_enter(menu_t *m);

menu_t *menu_handle_event(menu_t *m, app_event_t *evt, bsp_t *bsp);
void menu_draw(const menu_t *m, bsp_t *bsp);

#endif /* TD4_COMPLETE_APP_MENU_H_ */
