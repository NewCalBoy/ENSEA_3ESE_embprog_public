#include "time_bomb.h"

#include <stdio.h>

#define BOMB_BLINK_PERIOD_MS 500
#define BOMB_BLINK_COUNT 3

void time_bomb_init(time_bomb_t *tb, bool *led_on, uint8_t led_index, app_timers_t *timers, int timer_id) {
    tb->state = BOMB_IDLE;
    tb->blink_count = 0;
    tb->led_on = led_on;
    tb->led_index = led_index;
    tb->timers = timers;
    tb->timer_id = timer_id;
}

void time_bomb_enter(time_bomb_t *tb, bsp_t *bsp) {
    tb->state = BOMB_IDLE;
    tb->blink_count = 0;
    *tb->led_on = false;
    bsp->led_set(bsp, tb->led_index, false);
    bsp->neopixel_set_hsv(bsp, 0, 0, 0);
    app_timer_stop(tb->timers, tb->timer_id);
}

bool time_bomb_handle_event(time_bomb_t *tb, const app_event_t *evt, bsp_t *bsp, uint32_t now_ms) {
    switch (tb->state) {
        case BOMB_IDLE:
            if (evt->type == EVT_SW2_CLICK) {
                tb->state = BOMB_ARMED;
                tb->blink_count = 0;
                *tb->led_on = true;
                bsp->led_set(bsp, tb->led_index, *tb->led_on);
                app_timer_start(tb->timers, tb->timer_id, now_ms + BOMB_BLINK_PERIOD_MS);
            } else if (evt->type == EVT_SW1_CLICK) {
                return true; /* retour au menu */
            }
            return false;

        case BOMB_ARMED:
            if (evt->type != EVT_TIMER || evt->data != tb->timer_id) {
                return false; /* SW2 ignore ici : plus d'annulation possible */
            }

            *tb->led_on = !*tb->led_on;
            bsp->led_set(bsp, tb->led_index, *tb->led_on);
            if (*tb->led_on) {
                /* un cycle allume+eteint complet = un clignotement */
                tb->blink_count++;
            }

            /* Garde : encore des clignotements a faire, ou explosion. */
            if (tb->blink_count < BOMB_BLINK_COUNT || !*tb->led_on) {
                app_timer_start(tb->timers, tb->timer_id, now_ms + BOMB_BLINK_PERIOD_MS);
            } else {
                tb->state = BOMB_EXPLODED;
                *tb->led_on = false;
                bsp->led_set(bsp, tb->led_index, *tb->led_on);
                bsp->neopixel_set_hsv(bsp, 0, 0, 100); /* sat = 0 => blanc */
            }
            return false;

        case BOMB_EXPLODED:
            if (evt->type == EVT_SW1_CLICK) {
                return true; /* retour au menu */
            }
            return false;
    }
    return false;
}

void time_bomb_draw(const time_bomb_t *tb, bsp_t *bsp) {
    bsp->oled_clear(bsp);

    switch (tb->state) {
        case BOMB_IDLE:
            bsp->oled_draw_string(bsp, 4, 4, "TIME BOMB", true);
            bsp->oled_draw_string(bsp, 4, 28, "SW1 = BACK", true);
            bsp->oled_draw_string(bsp, 4, 44, "SW2 = ARM", true);
            break;

        case BOMB_ARMED: {
            char line[32];
            snprintf(line, sizeof(line), "ARMED... %d", BOMB_BLINK_COUNT - tb->blink_count);
            bsp->oled_draw_string(bsp, 4, 4, line, true);
            bsp->oled_draw_string(bsp, 4, 28, "no going back", true);
            break;
        }

        case BOMB_EXPLODED:
            bsp->oled_draw_string(bsp, 4, 4, "BOOM", true);
            bsp->oled_draw_string(bsp, 4, 44, "SW1 = BACK", true);
            break;
    }

    bsp->oled_show(bsp);
}
