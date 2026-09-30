/*
 * app.h
 *
 *  Created on: 30 sept. 2026
 *      Author: Bradley SERAPHIN
 */

#ifndef TD3_APP_APP_H_
#define TD3_APP_APP_H_

#include "bsp.h" /* direct coupling, for now */

typedef struct {
	uint32_t last_tick;
	bsp_t * bsp;
	bool led1_on, led2_on, led3_on;
} app_t;

void app_init(app_t *app, );
void app_process(app_t *app);

#endif /* TD3_APP_APP_H_ */
