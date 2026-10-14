#include "timers.h"

void app_timers_init(app_timers_t *timers) {
    for (int i = 0; i < APP_TIMER_COUNT; i++) {
        timers->slots[i].active = false;
    }
}

void app_timer_start(app_timers_t *timers, int id, uint32_t deadline_ms) {
    if (id < 0 || id >= APP_TIMER_COUNT) {
        return;
    }
    timers->slots[id].active = true;
    timers->slots[id].deadline_ms = deadline_ms;
}

void app_timer_stop(app_timers_t *timers, int id) {
    if (id < 0 || id >= APP_TIMER_COUNT) {
        return;
    }
    timers->slots[id].active = false;
}

void app_timers_process(app_timers_t *timers, uint32_t now_ms, event_queue_t *events) {
    for (int i = 0; i < APP_TIMER_COUNT; i++) {
        app_timer_t *t = &timers->slots[i];
        if (!t->active) {
            continue;
        }
        /* Comparaison signee sur la difference : sure au wraparound de
         * now_ms, meme principe que time_after()/time_before() dans le
         * noyau Linux. */
        if ((int32_t)(now_ms - t->deadline_ms) >= 0) {
            t->active = false; /* one-shot : a l'appelant de rearmer */
            event_queue_push(events, (app_event_t){.type = EVT_TIMER, .data = i});
        }
    }
}
