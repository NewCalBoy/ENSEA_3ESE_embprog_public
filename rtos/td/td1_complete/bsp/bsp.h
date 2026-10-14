/*
 * bsp.h
 *
 *  Created on: Sep 27, 2026
 *      Author: laurentf
 */

#ifndef TD3_COMPLETE_BSP_BSP_H_
#define TD3_COMPLETE_BSP_BSP_H_

#include <stdint.h>
#include <stdbool.h>
/* Fonctions à appeler dans le main */
void bsp_init(void);
void bsp_process(void);

void bsp_led_set(uint8_t led, bool on);
void bsp_neopixel_set_hsv(int hue, int sat, int val);
uint32_t bsp_get_tick_ms(void);

void bsp_oled_clear(void);
void bsp_oled_fill_rect(int x, int y, int w, int h, bool on);
void bsp_oled_draw_string(int x, int y, const char *str, bool on);
void bsp_oled_show(void);

#endif /* TD3_COMPLETE_BSP_BSP_H_ */
