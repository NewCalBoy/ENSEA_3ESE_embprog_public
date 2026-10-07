/*
 * event.c
 *
 *  Created on: Sep 29, 2026
 *      Author: laurentf
 */

#include "events.h"

void event_queue_init(event_queue_t *q) {
    q->head = 0;
    q->count = 0;
}

bool event_queue_push(event_queue_t *q, app_event_t evt) {
    if (q->count >= EVENT_QUEUE_CAPACITY) {
        return false;
    }
    int tail = (q->head + q->count) % EVENT_QUEUE_CAPACITY;
    q->items[tail] = evt;
    q->count++;
    return true;
}

bool event_queue_pull(event_queue_t *q, app_event_t *out) {
    if (q->count == 0) {
        return false;
    }
    *out = q->items[q->head];
    q->head = (q->head + 1) % EVENT_QUEUE_CAPACITY;
    q->count--;
    return true;
}
