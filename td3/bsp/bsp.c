/*
 * bsp.c
 *
 *  Created on: 30 sept. 2026
 *      Author: Bradley SERAPHIN
 */

#include "bsp.h"
#include "main.h"

static volatile bool sw1_pressed = false;
static volatile bool sw2_pressed = false;

typedef struct bsp_ctx_struct
{
	volatile bool sw1_pressed;
	volatile bool sw2_pressed;
} bsp_ctx_t;

static bsp_ctx_t bsp

void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
	if (pin == BTN_SW1_Pin)
	{
		sw1_pressed = true;
	}
	if (pin == BTN_SW2_Pin)
	{
		sw2_pressed = true;
	}
}


void bsp_init(bsp_t * bsp)
{
	bsp->ctx = (void*)&bsp_ctx;

	bsp_ctx.sw1_pressed = false;
	bsp_ctx.sw2_pressed = false;

	bsp->led_set = bsp_led_set;
	bsp->sw1_pressed = bsp_sw1_pressed;
	bsp->sw2_pressed = bsp_sw2_pressed;
	bsp->get_tick_ms = bsp_get_tick_ms;
}

void bsp_process(void)
{

}

static void bsp_led_set(uint8_t led, bool on)
{
	GPIO_PinState state = on ? GPIO_PIN_SET : GPIO_PIN_RESET;
	switch (led)
	{
	case 0:
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, state);
		break;
	case 1:
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, state);
		break;
	case 2:
		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, state);
		break;
	}
}


static bool bsp_sw1_pressed(void) {
	bool pressed = sw1_pressed;
	sw1_pressed = false;

	return pressed;
}

static bool bsp_sw2_pressed(void) {
	bool pressed = sw2_pressed;
	sw2_pressed = false;

	return pressed;
}

static uint32_t bsp_get_tick_ms(void)
{
	return HAL_GetTick();
}
