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
    EVT_TIMER, /* data : timer id */
} app_event_type_t;

typedef struct {
    app_event_type_t type;
    int32_t data;
} app_event_t;

#endif /* TD5_COMPLETE_APP_EVENTS_H_ */
