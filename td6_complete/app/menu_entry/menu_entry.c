/*
 * menu_entry.c
 *
 *  Created on: Sep 29, 2026
 *      Author: laurentf
 */

#include "menu_entry.h"

#include "led_toggle_entry.h"
#include "screen_switch_entry.h"
#include "hsv_component_entry.h"
#include "time_bomb_launch_entry.h"

/* Highlighted background + selection marker: identical for every concrete
 * entry, so it's shared here rather than duplicated in each *_draw below --
 * a bit like a protected helper a base class would offer its subclasses. */
void menu_entry_draw_row(bsp_t *bsp, int y, bool selected, bool editing, const char *line) {
	if (selected) {
		bsp->oled_fill_rect(bsp, 0, y, 127, 13, true);
		bsp->oled_draw_string(bsp, 4, y + 2, editing ? "*" : ">", false);
	}
	bsp->oled_draw_string(bsp, 16, y + 2, line, !selected);
}

/* --- menu_entry_t: polymorphic dispatch ---------------------------------- */
/* The one place in the whole program that knows every concrete entry kind
 * exists -- exactly like Shape_draw/Shape_area's switch in the slides.
 * menu_process/menu_draw below only ever call through these three
 * functions. */

menu_t *menu_entry_activate(menu_entry_t *self, bsp_t *bsp) {
	switch (self->kind) {
	case MENU_ENTRY_LED_TOGGLE:
		led_toggle_entry_toggle((led_toggle_entry_t *)self, bsp);
		return NULL;
	case MENU_ENTRY_SCREEN_SWITCH:
		return ((screen_switch_entry_t *)self)->target;
	case MENU_ENTRY_HSV_COMPONENT:
		return NULL; /* editable: activate() never runs, see menu_process */
	case MENU_ENTRY_TIME_BOMB_LAUNCH:
		return time_bomb_launch_entry_activate((time_bomb_launch_entry_t *)self, bsp);
	}
	return NULL;
}

void menu_entry_adjust(menu_entry_t *self, int32_t delta) {
	switch (self->kind) {
	case MENU_ENTRY_LED_TOGGLE:
	case MENU_ENTRY_SCREEN_SWITCH:
	case MENU_ENTRY_TIME_BOMB_LAUNCH:
		break; /* not editable, nothing to adjust */
	case MENU_ENTRY_HSV_COMPONENT:
		hsv_component_entry_adjust((hsv_component_entry_t *)self, delta);
		break;
	}
}

void menu_entry_draw(const menu_entry_t *self, bsp_t *bsp, int y, bool selected, bool editing) {
	switch (self->kind) {
	case MENU_ENTRY_LED_TOGGLE:
		led_toggle_entry_draw((const led_toggle_entry_t *)self, bsp, y, selected);
		break;
	case MENU_ENTRY_SCREEN_SWITCH:
		screen_switch_entry_draw((const screen_switch_entry_t *)self, bsp, y, selected);
		break;
	case MENU_ENTRY_HSV_COMPONENT:
		hsv_component_entry_draw((const hsv_component_entry_t *)self, bsp, y, selected, editing);
		break;
	case MENU_ENTRY_TIME_BOMB_LAUNCH:
		time_bomb_launch_entry_draw((const time_bomb_launch_entry_t *)self, bsp, y, selected);
		break;
	}
}
