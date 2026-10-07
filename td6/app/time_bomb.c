#include "time_bomb.h"

#define BOMB_BLINK_PERIOD_MS 500

void time_bomb_init(time_bomb_t *tb, bool *led_on, uint8_t led_index, app_timers_t *timers, int timer_id) {
    tb->led_on = led_on;
    tb->led_index = led_index;
    tb->timers = timers;
    tb->timer_id = timer_id;
}

void time_bomb_enter(time_bomb_t *tb, bsp_t *bsp) {
    *tb->led_on = false;
    bsp->led_set(bsp, tb->led_index, false);
    bsp->neopixel_set_hsv(bsp, 0, 0, 0);
    app_timer_start(tb->timers, tb->timer_id, bsp->get_tick_ms(bsp) + BOMB_BLINK_PERIOD_MS);
}

bool time_bomb_handle_event(time_bomb_t *tb, const app_event_t *evt, bsp_t *bsp, uint32_t now_ms) {
    if (evt->type == EVT_SW1_CLICK) {
        app_timer_stop(tb->timers, tb->timer_id);
        return true; /* retour au menu */
    }

    if (evt->type == EVT_TIMER && evt->data == tb->timer_id) {
        *tb->led_on = !*tb->led_on;
        bsp->led_set(bsp, tb->led_index, *tb->led_on);
        app_timer_start(tb->timers, tb->timer_id, now_ms + BOMB_BLINK_PERIOD_MS);
    }

    return false;
}

void time_bomb_draw(const time_bomb_t *tb, bsp_t *bsp) {
    (void)tb;

    bsp->oled_clear(bsp);
    bsp->oled_draw_string(bsp, 4, 4, "NOT IMPLEMENTED YET", true);
    bsp->oled_draw_string(bsp, 4, 44, "SW1 = BACK", true);
    bsp->oled_show(bsp);
}
