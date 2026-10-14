/*
 * bsp.c
 *
 *  Created on: Sep 27, 2026
 *      Author: laurentf
 */

#include "bsp.h"

#include <string.h>	// memcpy, strlen

#include "main.h"
#include "tim.h"

#include "FreeRTOS.h"
#include "task.h"

#include "app.h"
#include "debounce.h"
#include "neopixel.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"

#define BSP_OLED_WIDTH  128
#define BSP_OLED_HEIGHT 64

static const SSD1306_Font_t *const OLED_FONT = &Font_6x8;

#define BSP_TASK_STACK_DEPTH 256	/* en mots de 32 bits */
#define BSP_POLL_PERIOD_MS   5		/* scrutation des boutons et de l'encodeur */

/* Etat interne (singleton) */
typedef struct {
	TaskHandle_t task;			/* gardé dans le contexte (utile pour les notifications) */

	bool display_present;

	int32_t encoder_last_count;

	debounce_t deb_sw1;
	debounce_t deb_sw2;
	debounce_t deb_enc;
} bsp_ctx_t;

static bsp_ctx_t bsp_ctx;

static void bsp_task(void *unused);
static void bsp_process(void);

/* Init à appeler dans le main */
void bsp_init(unsigned int task_priority) {
	bsp_ctx_t * ctx = &bsp_ctx;

    /* SSD1306 OLED Display (I2C1) */
    if (HAL_OK == HAL_I2C_IsDeviceReady(&hi2c1, SSD1306_I2C_ADDR, 3, 1000)) {
        ctx->display_present = true;
        ssd1306_Init();
    }

    /* Encoder */
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1);
    ctx->encoder_last_count = (int32_t)(int16_t)__HAL_TIM_GET_COUNTER(&htim3);

    /* Boutons : scrutes dans bsp_process, pas d'interruption */
    debounce_init(&ctx->deb_sw1);
    debounce_init(&ctx->deb_sw2);
    debounce_init(&ctx->deb_enc);

    /* Neopixel eteint au demarrage */
    np_led_set_all_RGB(0, 0, 0);
    np_led_render();
    HAL_Delay(10); // Le driver DMA a besoin de temps avant le prochain appel

    /* Pas d'objet actif ici (pas de file) : une tâche périodique, pilotée par le temps */
    BaseType_t ok = xTaskCreate(bsp_task, "BSP", BSP_TASK_STACK_DEPTH,
    		NULL, task_priority, &ctx->task);
    configASSERT(ok == pdPASS);
}

static void bsp_task(void *unused) {
	(void)unused;	/* la BSP est un singleton : pas besoin de passer le contexte */

	for (;;) {
		bsp_process();
		vTaskDelay(pdMS_TO_TICKS(BSP_POLL_PERIOD_MS));	/* Bloquée : le CPU est libre */
	}
}

static void bsp_process(void) {
	bsp_ctx_t * ctx = &bsp_ctx;

    int32_t count = (int32_t)(int16_t)__HAL_TIM_GET_COUNTER(&htim3);
    int32_t delta = count - ctx->encoder_last_count;
    ctx->encoder_last_count = count;
    if (delta != 0) {
        app_post_event((app_event_t){.type = EVT_ENCODER_ROTATE, .data = delta});
    }

    uint32_t now = HAL_GetTick();

    bool sw1_pressed = (HAL_GPIO_ReadPin(BTN_SW1_GPIO_Port, BTN_SW1_Pin) == GPIO_PIN_RESET);
    if (debounce_process(&ctx->deb_sw1, sw1_pressed, now)) {
        app_post_event((app_event_t){.type = EVT_SW1_CLICK});
    }

    bool sw2_pressed = (HAL_GPIO_ReadPin(BTN_SW2_GPIO_Port, BTN_SW2_Pin) == GPIO_PIN_RESET);
    if (debounce_process(&ctx->deb_sw2, sw2_pressed, now)) {
        app_post_event((app_event_t){.type = EVT_SW2_CLICK});
    }

    bool enc_pressed = (HAL_GPIO_ReadPin(ENCODER_PB_GPIO_Port, ENCODER_PB_Pin) == GPIO_PIN_RESET);
    if (debounce_process(&ctx->deb_enc, enc_pressed, now)) {
        app_post_event((app_event_t){.type = EVT_ENCODER_CLICK});
    }
}

/* h: 0..359, s/v: 0..255 */
static void hsv_to_rgb(int h, int s, int v, uint8_t *r, uint8_t *g, uint8_t *b) {
	h = ((h % 360) + 360) % 360;
	if (s <= 0) {
		*r = *g = *b = (uint8_t)v;
		return;
	}

	int region = h / 60;
	int rem = h % 60;
	int p = (v * (255 - s)) / 255;
	int q = (v * (255 - (s * rem) / 60)) / 255;
	int t = (v * (255 - (s * (60 - rem)) / 60)) / 255;

	switch (region) {
	case 0: *r = (uint8_t)v; *g = (uint8_t)t; *b = (uint8_t)p; break;
	case 1: *r = (uint8_t)q; *g = (uint8_t)v; *b = (uint8_t)p; break;
	case 2: *r = (uint8_t)p; *g = (uint8_t)v; *b = (uint8_t)t; break;
	case 3: *r = (uint8_t)p; *g = (uint8_t)q; *b = (uint8_t)v; break;
	case 4: *r = (uint8_t)t; *g = (uint8_t)p; *b = (uint8_t)v; break;
	default: *r = (uint8_t)v; *g = (uint8_t)p; *b = (uint8_t)q; break;
	}
}

void bsp_neopixel_set_hsv(int hue, int sat, int val) {
	uint8_t r, g, b;
	hsv_to_rgb(hue, sat, val, &r, &g, &b);
	np_led_set_all_RGB(r, g, b);
	np_led_render();
}

void bsp_led_set(uint8_t led, bool on) {
	GPIO_PinState state = on ? GPIO_PIN_SET : GPIO_PIN_RESET;
	switch (led) {
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

uint32_t bsp_get_tick_ms(void) {
	return HAL_GetTick();
}

void bsp_oled_clear(void) {
	bsp_ctx_t * ctx = &bsp_ctx;

    if (!ctx->display_present) {
        return;
    }
    ssd1306_Fill(Black);
}

void bsp_oled_fill_rect(int x, int y, int w, int h, bool on) {
	bsp_ctx_t * ctx = &bsp_ctx;

    if (!ctx->display_present) {
        return;
    }
    if (w <= 0 || h <= 0) {
        return;
    }

    int x1 = x < 0 ? 0 : x;
    int y1 = y < 0 ? 0 : y;
    int x2 = x + w - 1;
    int y2 = y + h - 1;
    if (x2 >= BSP_OLED_WIDTH) {
        x2 = BSP_OLED_WIDTH - 1;
    }
    if (y2 >= BSP_OLED_HEIGHT) {
        y2 = BSP_OLED_HEIGHT - 1;
    }
    if (x1 > x2 || y1 > y2) {
        return;
    }

    ssd1306_FillRectangle((uint8_t)x1, (uint8_t)y1, (uint8_t)x2, (uint8_t)y2, on ? White : Black);
}

void bsp_oled_draw_string(int x, int y, const char *str, bool on) {
	bsp_ctx_t * ctx = &bsp_ctx;

    if (!ctx->display_present) {
        return;
    }
    if (x < 0 || y < 0 || x >= BSP_OLED_WIDTH || y >= BSP_OLED_HEIGHT) {
        return;
    }

    char buf[32];
    size_t n = strlen(str);
    if (n >= sizeof(buf)) {
        n = sizeof(buf) - 1;
    }
    memcpy(buf, str, n);
    buf[n] = '\0';

    ssd1306_SetCursor((uint8_t)x, (uint8_t)y);
    ssd1306_WriteString(buf, *OLED_FONT, on ? White : Black);
}

void bsp_oled_show(void) {
	bsp_ctx_t * ctx = &bsp_ctx;

    if (!ctx->display_present) {
        return;
    }
    ssd1306_UpdateScreen();
}

