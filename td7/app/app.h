/*
 * app.h
 *
 *  Created on: Sep 27, 2026
 *      Author: laurentf
 */

#ifndef TD3_COMPLETE_APP_APP_H_
#define TD3_COMPLETE_APP_APP_H_

#include "bsp.h"

#include "events.h"
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
	bsp_t * bsp;
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

void app_init(app_t *app, bsp_t *bsp);
void app_process(app_t *app);


#endif /* TD3_COMPLETE_APP_APP_H_ */
