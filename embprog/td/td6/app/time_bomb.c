#include "time_bomb.h"

#define BOMB_BLINK_PERIOD_MS 500
#define BOMB_BLINK_COUNT 3

void time_bomb_init(time_bomb_t *tb, bool *led_on, uint8_t led_index, app_timers_t *timers, int timer_id) {
	tb->state=TIME_BOMB_IDLE;
	tb->blink_count=0;
	tb->led_on = led_on;
	tb->led_index = led_index;
	tb->timers = timers;
	tb->timer_id = timer_id;
}

void time_bomb_enter(time_bomb_t *tb, bsp_t *bsp) {
	tb->state=TIME_BOMB_IDLE;
	tb->blink_count=0;
	*tb->led_on = false;
	bsp->led_set(bsp, tb->led_index, false);
	bsp->neopixel_set_hsv(bsp, 0, 0, 0);
	app_timer_stop(tb->timers, tb->timer_id);
	//app_timer_start(tb->timers, tb->timer_id, bsp->get_tick_ms(bsp) + BOMB_BLINK_PERIOD_MS);
}

bool time_bomb_handle_event(time_bomb_t *tb, const app_event_t *evt, bsp_t *bsp, uint32_t now_ms) {
	switch(tb->state){
	case TIME_BOMB_IDLE:
		bsp->led_set(bsp, tb->led_index, false);
		bsp->neopixel_set_hsv(bsp, 0, 0, 0);

		if (evt->type == EVT_SW2_CLICK) {
			tb->state=TIME_BOMB_ARMED;
			tb->blink_count = 0;
			*tb->led_on = true;
			bsp->led_set(bsp, tb->led_index, *tb->led_on);
			app_timer_start(tb->timers, tb->timer_id, now_ms + BOMB_BLINK_PERIOD_MS);
		}

		if (evt->type == EVT_SW1_CLICK) {
			return true; /* retour au menu */
		}


	case TIME_BOMB_ARMED:
		if (evt->type != EVT_TIMER || evt->data != tb->timer_id) {
			return false; /* SW2 ignore ici : plus d'annulation possible */
		}
		*tb->led_on = !*tb->led_on;
		bsp->led_set(bsp, tb->led_index, *tb->led_on);
		if (*tb->led_on) {
			tb->blink_count++;
		}
		if(tb->blink_count<BOMB_BLINK_COUNT || !*tb->led_on)
		{
			app_timer_start(tb->timers, tb->timer_id, now_ms + BOMB_BLINK_PERIOD_MS);
		}
		else
		{
			tb->state=TIME_BOMB_EXPLOSED;
			*tb->led_on = false;
			bsp->led_set(bsp, tb->led_index, *tb->led_on);
			bsp->neopixel_set_hsv(bsp, 0, 0, 100);

		}

	case TIME_BOMB_EXPLOSED:
		if (evt->type == EVT_SW1_CLICK) {
			return true; /* retour au menu */
		}

}

return false;
}

void time_bomb_draw(const time_bomb_t *tb, bsp_t *bsp) {
	bsp->oled_clear(bsp);

	switch(tb->state){
	case TIME_BOMB_IDLE:
		bsp->oled_draw_string(bsp, 4, 4, "TIME BOMB NOT ARMED", true);
		bsp->oled_draw_string(bsp, 4, 28, "SW1 = BACK", true);
		bsp->oled_draw_string(bsp, 4, 44, "SW2 = ARMED", true);
		bsp->oled_show(bsp);
		break;

	case TIME_BOMB_ARMED:
		bsp->oled_draw_string(bsp, 4, 4, "TIME BOMB ARMED", true);
		bsp->oled_show(bsp);
		break;

	case TIME_BOMB_EXPLOSED:
		bsp->oled_draw_string(bsp, 4, 4, "TIME BOMB EXPLOSED", true);
		bsp->oled_draw_string(bsp, 4, 44, "SW1 = BACK", true);
		bsp->oled_show(bsp);
		break;

	}

}
