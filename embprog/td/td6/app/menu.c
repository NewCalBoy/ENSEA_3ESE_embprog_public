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
menu_t *menu_handle_event(menu_t *m, app_event_t *evt, bsp_t *bsp) {
	menu_entry_t *entry = m->entries[m->cursor];

    if (m->editing) {
        switch (evt->type) {
            case EVT_ENCODER_ROTATE:
                menu_entry_adjust(entry, evt->data);
                return NULL;
            case EVT_ENCODER_CLICK:
                m->editing = false;
                return NULL;
            default:
                return NULL;	// Ignore other events for now
        }
    }

    switch (evt->type) {
        case EVT_ENCODER_ROTATE:
    		m->cursor += (int)evt->data;
    		if (m->cursor < 0) {
    			m->cursor = 0;
    		}
    		if (m->cursor >= m->count) {
    			m->cursor = m->count - 1;
    		}
    		entry = m->entries[m->cursor];

            return NULL;
        case EVT_ENCODER_CLICK:
            if (entry->kind == MENU_ENTRY_HSV_COMPONENT) {
                m->editing = true;
            } else {
                return menu_entry_activate(entry, bsp);
            }
            return NULL;
        default:
            return NULL;
    }
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
