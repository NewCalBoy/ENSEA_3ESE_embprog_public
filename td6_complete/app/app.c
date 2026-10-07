/*
 * app.c
 *
 *  Created on: Sep 27, 2026
 *      Author: laurentf
 */

#include "app.h"

#include <stddef.h>

static const char *const LED_LABELS[3] = {"LED1", "LED2", "LED3"};

#define BOMB_TIMER_ID 0

static led_toggle_entry_t led_entries[3];
static screen_switch_entry_t leds_entry;
static screen_switch_entry_t neopixel_entry;
static time_bomb_launch_entry_t time_bomb_entry;
static screen_switch_entry_t led_back_entry;

static hsv_component_entry_t hue_entry;
static hsv_component_entry_t sat_entry;
static hsv_component_entry_t val_entry;
static screen_switch_entry_t back_entry;

void app_init(app_t *app, bsp_t *bsp) {
	app->bsp = bsp;

	event_queue_init(&app->events);
	app_timers_init(&app->timers);

	app->mode = APP_MODE_MENU;

	menu_init(&app->main_menu);
	screen_switch_entry_init(&leds_entry, "LEDS", &app->led_menu);
	menu_add_entry(&app->main_menu, &leds_entry.super);
	screen_switch_entry_init(&neopixel_entry, "NEOPIXEL", &app->hsv_menu);
	menu_add_entry(&app->main_menu, &neopixel_entry.super);
	time_bomb_launch_entry_init(&time_bomb_entry, "TIME BOMB", &app->mode, &app->bomb);
	menu_add_entry(&app->main_menu, &time_bomb_entry.super);

	menu_init(&app->led_menu);
	for (int i = 0; i < 3; i++) {
		app->led_on[i] = false;
		led_toggle_entry_init(&led_entries[i], LED_LABELS[i], &app->led_on[i], (uint8_t)i);
		menu_add_entry(&app->led_menu, &led_entries[i].super);
	}
	screen_switch_entry_init(&led_back_entry, "BACK", &app->main_menu);
	menu_add_entry(&app->led_menu, &led_back_entry.super);

	/* La bombe pilote LED1 -- meme champ (app->led_on[0]) que la page LEDS. */
	time_bomb_init(&app->bomb, &app->led_on[0], 0, &app->timers, BOMB_TIMER_ID);

	app->hue = 0;
	app->sat = 255;
	app->val = 30;
	menu_init(&app->hsv_menu);
	hsv_component_entry_init(&hue_entry, "HUE", &app->hue, 0, 359, 1);
	hsv_component_entry_init(&sat_entry, "SAT", &app->sat, 0, 255, 1);
	hsv_component_entry_init(&val_entry, "VAL", &app->val, 0, 255, 1);
	screen_switch_entry_init(&back_entry, "BACK", &app->main_menu);
	menu_add_entry(&app->hsv_menu, &hue_entry.super);
	menu_add_entry(&app->hsv_menu, &sat_entry.super);
	menu_add_entry(&app->hsv_menu, &val_entry.super);
	menu_add_entry(&app->hsv_menu, &back_entry.super);

	app->active = &app->main_menu;

	bsp->neopixel_set_hsv(bsp, app->hue, app->sat, app->val);
	menu_draw(app->active, bsp);
}

void app_process(app_t *app) {
	bsp_t * bsp = app->bsp;
	app_event_t evt;

	uint32_t now = bsp->get_tick_ms(bsp);
	app_timers_process(&app->timers, now, &app->events);

	while (event_queue_pull(&app->events, &evt)) {
		if (app->mode == APP_MODE_TIME_BOMB) {
			if (time_bomb_handle_event(&app->bomb, &evt, bsp, now)) {
				app->mode = APP_MODE_MENU;
			}
		} else {
			menu_t *next = menu_handle_event(app->active, &evt, bsp);

			if (next != NULL) {
				app->active = next;
				menu_enter(app->active);
			}
		}

		/* La bombe pilote le neopixel elle-meme (blanc a l'explosion) --
		 * ne pas l'ecraser avec HUE/SAT/VAL tant qu'elle a la main. */
		if (app->mode == APP_MODE_MENU) {
			bsp->neopixel_set_hsv(bsp, app->hue, app->sat, app->val);
			menu_draw(app->active, bsp);
		} else {
			time_bomb_draw(&app->bomb, bsp);
		}
	}
}
