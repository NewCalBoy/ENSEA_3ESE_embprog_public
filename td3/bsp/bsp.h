/*
 * bsp.h
 *
 *  Created on: 30 sept. 2026
 *      Author: Bradley SERAPHIN
 */

#ifndef TD3_BSP_BSP_H_
#define TD3_BSP_BSP_H_

#include <stdint.h>
#include <stdbool.h>

//On n'inclus pas la HAL ou le main.h ici
//De manière générale, aucune info ST ou n'importe quelle autre plateforme

typedef struct bsp_struct bsp_t;
struct bsp_struct
{
	void * ctx;//pointeur générique

	void (*led_set)(bsp_t * bsp, uint8_t led, bool on);
	bool (*sw1_pressed)(bsp_t * bsp);
	bool (*sw2_pressed)(bsp_t * bsp);
	uint32_t (*get_tick_ms)(bsp_t * bsp);
};

void bsp_init(void);
void bsp_process(void);

//void bsp_led_set(uint8_t led, bool on);
//bool bsp_sw1_pressed(void);
//bool bsp_sw2_pressed(void);
//uint32_t bsp_get_tick_ms(void);

static void bsp_led_set(bsp_t * bsp, uint8_t led, bool on);
static bool bsp_sw1_pressed(bsp_t * bsp);
static bool bsp_sw2_pressed(bsp_t * bsp);
static uint32_t bsp_get_tick_ms(bsp_t * bsp);

#endif /* TD3_BSP_BSP_H_ */
