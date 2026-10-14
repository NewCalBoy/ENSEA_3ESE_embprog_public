/*
 * event.h
 *
 *  Created on: Sep 29, 2026
 *      Author: laurentf
 */

#ifndef TD5_COMPLETE_APP_EVENTS_H_
#define TD5_COMPLETE_APP_EVENTS_H_

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    EVT_ENCODER_ROTATE, /* data : delta */
    EVT_ENCODER_CLICK,
    EVT_SW1_CLICK,
    EVT_SW2_CLICK,
} app_event_type_t;

typedef struct {
    app_event_type_t type;
    int32_t data;
} app_event_t;

#define EVENT_QUEUE_CAPACITY 16

typedef struct {
    app_event_t items[EVENT_QUEUE_CAPACITY];
    int head;
    int count;
} event_queue_t;

void event_queue_init(event_queue_t *q);
bool event_queue_push(event_queue_t *q, app_event_t evt);
bool event_queue_pull(event_queue_t *q, app_event_t *out);

#endif /* TD5_COMPLETE_APP_EVENTS_H_ */
