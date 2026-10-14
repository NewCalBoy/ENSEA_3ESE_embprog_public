/*
 * app.c
 *
 *  Created on: Sep 27, 2026
 *      Author: laurentf
 */

#include "app.h"

#include <stddef.h>

#include "bsp.h"
#include "timers.h"
#include "app_mode.h"
#include "time_bomb.h"
#include "runner.h"
#include "menu.h"
#include "led_toggle_entry.h"
#include "screen_switch_entry.h"
#include "hsv_component_entry.h"
#include "time_bomb_launch_entry.h"
#include "runner_launch_entry.h"

typedef struct {
	event_queue_t events;
	app_timers_t timers;

	app_mode_t mode;
	time_bomb_t bomb;
	runner_t runner;

	menu_t main_menu;
	menu_t led_menu;
	menu_t hsv_menu;
	menu_t *active;

	bool led_on[3];

	int hue;
	int sat;
	int val;
} app_t;

/* Singleton */
static app_t app;

static const char *const LED_LABELS[3] = {"LED1", "LED2", "LED3"};

#define BOMB_TIMER_ID 0
#define RUNNER_TIMER_ID 1

static led_toggle_entry_t led_entries[3];
static screen_switch_entry_t leds_entry;
static screen_switch_entry_t neopixel_entry;
static time_bomb_launch_entry_t time_bomb_entry;
static runner_launch_entry_t runner_entry;
static screen_switch_entry_t led_back_entry;

static hsv_component_entry_t hue_entry;
static hsv_component_entry_t sat_entry;
static hsv_component_entry_t val_entry;
static screen_switch_entry_t back_entry;

void app_init(void) {
	event_queue_init(&app.events);
	app_timers_init(&app.timers);

	app.mode = APP_MODE_MENU;

	menu_init(&app.main_menu);
	screen_switch_entry_init(&leds_entry, "LEDS", &app.led_menu);
	menu_add_entry(&app.main_menu, &leds_entry.super);
	screen_switch_entry_init(&neopixel_entry, "NEOPIXEL", &app.hsv_menu);
	menu_add_entry(&app.main_menu, &neopixel_entry.super);
	time_bomb_launch_entry_init(&time_bomb_entry, "TIME BOMB", &app.mode, &app.bomb);
	menu_add_entry(&app.main_menu, &time_bomb_entry.super);
	runner_launch_entry_init(&runner_entry, "RUNNER", &app.mode, &app.runner);
	menu_add_entry(&app.main_menu, &runner_entry.super);

	menu_init(&app.led_menu);
	for (int i = 0; i < 3; i++) {
		app.led_on[i] = false;
		led_toggle_entry_init(&led_entries[i], LED_LABELS[i], &app.led_on[i], (uint8_t)i);
		menu_add_entry(&app.led_menu, &led_entries[i].super);
	}
	screen_switch_entry_init(&led_back_entry, "BACK", &app.main_menu);
	menu_add_entry(&app.led_menu, &led_back_entry.super);

	/* La bombe pilote LED1 -- meme champ (app.led_on[0]) que la page LEDS. */
	time_bomb_init(&app.bomb, &app.led_on[0], 0, &app.timers, BOMB_TIMER_ID);

	runner_init(&app.runner, &app.timers, RUNNER_TIMER_ID);

	app.hue = 0;
	app.sat = 255;
	app.val = 30;
	menu_init(&app.hsv_menu);
	hsv_component_entry_init(&hue_entry, "HUE", &app.hue, 0, 359, 1);
	hsv_component_entry_init(&sat_entry, "SAT", &app.sat, 0, 255, 1);
	hsv_component_entry_init(&val_entry, "VAL", &app.val, 0, 255, 1);
	screen_switch_entry_init(&back_entry, "BACK", &app.main_menu);
	menu_add_entry(&app.hsv_menu, &hue_entry.super);
	menu_add_entry(&app.hsv_menu, &sat_entry.super);
	menu_add_entry(&app.hsv_menu, &val_entry.super);
	menu_add_entry(&app.hsv_menu, &back_entry.super);

	app.active = &app.main_menu;

	bsp_neopixel_set_hsv(app.hue, app.sat, app.val);
	menu_draw(app.active);
}

bool app_post_event(app_event_t evt) {
	return event_queue_push(&app.events, evt);
}

void app_process(void) {
	app_event_t evt;

	uint32_t now = bsp_get_tick_ms();
	app_timers_process(&app.timers, now);

	while (event_queue_pull(&app.events, &evt)) {
		if (app.mode == APP_MODE_TIME_BOMB) {
			if (time_bomb_handle_event(&app.bomb, &evt, now)) {
				app.mode = APP_MODE_MENU;
			}
		} else if (app.mode == APP_MODE_RUNNER) {
			if (runner_handle_event(&app.runner, &evt, now)) {
				app.mode = APP_MODE_MENU;
			}
		} else {
			menu_t *next = menu_handle_event(app.active, &evt);

			if (next != NULL) {
				app.active = next;
				menu_enter(app.active);
			}
		}

		/* La bombe pilote le neopixel elle-meme (blanc a l'explosion) --
		 * ne pas l'ecraser avec HUE/SAT/VAL tant qu'elle a la main
		 * (idem pour le runner). */
		if (app.mode == APP_MODE_MENU) {
			bsp_neopixel_set_hsv(app.hue, app.sat, app.val);
			menu_draw(app.active);
		} else if (app.mode == APP_MODE_RUNNER) {
			runner_draw(&app.runner);
		} else {
			time_bomb_draw(&app.bomb);
		}
	}
}
