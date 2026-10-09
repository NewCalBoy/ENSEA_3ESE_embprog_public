/*
 * menu_entry.h
 *
 *  Created on: Sep 29, 2026
 *      Author: laurentf
 */

#ifndef TD4_COMPLETE_APP_MENU_ENTRY_H_
#define TD4_COMPLETE_APP_MENU_ENTRY_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "bsp.h"

/* MENU ENTRY -- base "class" : a tag saying which concrete entry this
 * really is, plus what every entry has in common. Every concrete entry
 * embeds this as its *first* field ("inheritance"), so a pointer to a
 * concrete entry is always also a valid menu_entry_t*. */

typedef struct menu menu_t; /* defined below; entries only need the pointer type here */

typedef enum {
	MENU_ENTRY_LED_TOGGLE,
	MENU_ENTRY_SCREEN_SWITCH,
	MENU_ENTRY_HSV_COMPONENT,
	MENU_ENTRY_TIME_BOMB_LAUNCH,
	MENU_ENTRY_RUNNER_LAUNCH,
} menu_entry_kind_t;

typedef struct {
	menu_entry_kind_t kind;
	const char *label;
} menu_entry_t;

/* Polymorphic operations: one call site per operation, declared once here on
 * the base type. Each dispatches on `kind` (see menu.c) to the matching
 * concrete entry's behaviour -- callers (menu_process/menu_draw) never need
 * to know which concrete entry they're talking to. */
menu_t *menu_entry_activate(menu_entry_t *self, bsp_t *bsp);
void menu_entry_adjust(menu_entry_t *self, int32_t delta);
void menu_entry_draw(const menu_entry_t *self, bsp_t *bsp, int y, bool selected, bool editing);

/* Protected helper for concrete entries: highlighted background + selection
 * marker + text, identical for every kind of entry. */
void menu_entry_draw_row(bsp_t *bsp, int y, bool selected, bool editing, const char *line);

#endif /* TD4_COMPLETE_APP_MENU_ENTRY_H_ */
