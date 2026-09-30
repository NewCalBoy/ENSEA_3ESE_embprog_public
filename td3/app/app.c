/*
 * app.c
 *
 *  Created on: 30 sept. 2026
 *      Author: Bradley SERAPHIN
 */

#include "app.h"

void app_init(app_t *app)
{
	app->last_tick = 0;
}

void app_process(app_t *app, bsp_t * bsp)
{
	uint32_t tick = bsp_get_tick_ms();
	if (tick- app->last_tick >= 250)
	{
		app->last_tick = tick;
		app->led1_on = !app->led1_on;
		bsp_led_set(0, app->led1_on);
	}
	/* led2_on/led3_on on bsp_sw1_pressed()/bsp_sw2_pressed(): same shape */
	if (bsp_sw1_pressed())
	{
		app->led2_on = !app->led2_on;
		bsp_led_set(1,app->led2_on);
	}
	if (bsp_sw2_pressed())
	{
		app->led3_on = !app->led3_on;
		bsp_led_set(2,app->led3_on);
	}
}
