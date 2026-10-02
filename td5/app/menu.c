/*
 * menu.c
 *
 *  Created on: Sep 29, 2026
 *      Author: laurentf
 */

#include "menu.h"

/* --- MENU LOGIC ----------------------------------------------------------- */

void menu_init(menu_t *m) {
	m->count = 0;
	m->cursor = 0;
	m->editing = false;
}

void menu_add_entry(menu_t *m, menu_entry_t *entry) {
	m->entries[m->count++] = entry;
}

void menu_enter(menu_t *m) {
	m->cursor = 0;
	m->editing = false;
}

menu_t *menu_process(menu_t *m, bsp_t *bsp) {
	int32_t delta = bsp->encoder_get_delta(bsp);
	menu_entry_t *entry = m->entries[m->cursor];

	if (m->editing) {
		if (delta != 0) {
			menu_entry_adjust(entry, delta);
		}
		if (bsp->enc_pressed(bsp)) {
			m->editing = false;
		}
		return NULL;
	}

	if (delta != 0) {
		m->cursor += (int)delta;
		if (m->cursor < 0) {
			m->cursor = 0;
		}
		if (m->cursor >= m->count) {
			m->cursor = m->count - 1;
		}
		entry = m->entries[m->cursor];
	}

	if (bsp->enc_pressed(bsp)) {
		if (entry->kind == MENU_ENTRY_HSV_COMPONENT) {	// Editable
			m->editing = true;
			return NULL;
		}
		return menu_entry_activate(entry, bsp);
	}

	return NULL;
}

void menu_draw(const menu_t *m, bsp_t *bsp) {
	bsp->oled_clear(bsp);

	for (int row = 0; row < m->count; row++) {
		int y = 4 + row * 15;
		bool selected = (row == m->cursor);
		bool editing = selected && m->editing;
		menu_entry_draw(m->entries[row], bsp, y, selected, editing);
	}

	bsp->oled_show(bsp);
}
