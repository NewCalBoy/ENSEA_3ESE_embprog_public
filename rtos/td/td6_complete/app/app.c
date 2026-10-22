/*
 * app.c
 *
 *  Created on: Sep 27, 2026
 *      Author: laurentf
 */

#include "app.h"

#include <stdio.h>	// printf TODO remove
#include <stddef.h>
#include <stdlib.h> // atoi

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "bsp.h"
#include "app_timers.h"
#include "app_mode.h"
#include "time_bomb.h"
#include "runner.h"
#include "menu.h"
#include "led_toggle_entry.h"
#include "screen_switch_entry.h"
#include "hsv_component_entry.h"
#include "time_bomb_launch_entry.h"
#include "runner_launch_entry.h"
#include "shell.h"

#define APP_TASK_STACK_DEPTH 256	/* en mots de 32 bits */
#define APP_EVENT_QUEUE_LENGTH 16

typedef struct {
	TaskHandle_t task;			/* gardé dans l'objet (utile pour les notifications) */
	QueueHandle_t events;		/* file privée de l'objet actif */
	app_timers_t timers;

	app_mode_t mode;
	time_bomb_t bomb;
	runner_t runner;

	menu_t main_menu;
	menu_t led_menu;
	menu_t hsv_menu;
	menu_t *active;

	bool led_on[3];

	int hue;
	int sat;
	int val;

	shell_t shell;
} app_t;

/* Singleton */
static app_t app;

static const char *const LED_LABELS[3] = {"LED1", "LED2", "LED3"};

#define BOMB_TIMER_ID 0
#define RUNNER_TIMER_ID 1

static led_toggle_entry_t led_entries[3];
static screen_switch_entry_t leds_entry;
static screen_switch_entry_t neopixel_entry;
static time_bomb_launch_entry_t time_bomb_entry;
static runner_launch_entry_t runner_entry;
static screen_switch_entry_t led_back_entry;

static hsv_component_entry_t hue_entry;
static hsv_component_entry_t sat_entry;
static hsv_component_entry_t val_entry;
static screen_switch_entry_t back_entry;

static void app_task(void *unused);
static void app_handle_event(app_event_t *evt);
static int sh_led(int argc, char ** argv);
static int sh_encoder(int argc, char ** argv);
static int sh_neopixel(int argc, char ** argv);
static int sh_switch(int argc, char ** argv);

void app_init(unsigned int task_priority) {
	app_timers_init(&app.timers);

	app.mode = APP_MODE_MENU;

	menu_init(&app.main_menu);
	screen_switch_entry_init(&leds_entry, "LEDS", &app.led_menu);
	menu_add_entry(&app.main_menu, &leds_entry.super);
	screen_switch_entry_init(&neopixel_entry, "NEOPIXEL", &app.hsv_menu);
	menu_add_entry(&app.main_menu, &neopixel_entry.super);
	time_bomb_launch_entry_init(&time_bomb_entry, "TIME BOMB", &app.mode, &app.bomb);
	menu_add_entry(&app.main_menu, &time_bomb_entry.super);
	runner_launch_entry_init(&runner_entry, "RUNNER", &app.mode, &app.runner);
	menu_add_entry(&app.main_menu, &runner_entry.super);

	menu_init(&app.led_menu);
	for (int i = 0; i < 3; i++) {
		app.led_on[i] = false;
		led_toggle_entry_init(&led_entries[i], LED_LABELS[i], &app.led_on[i], (uint8_t)i);
		menu_add_entry(&app.led_menu, &led_entries[i].super);
	}
	screen_switch_entry_init(&led_back_entry, "BACK", &app.main_menu);
	menu_add_entry(&app.led_menu, &led_back_entry.super);

	/* La bombe pilote LED1 -- meme champ (app.led_on[0]) que la page LEDS. */
	time_bomb_init(&app.bomb, &app.led_on[0], 0, &app.timers, BOMB_TIMER_ID);

	runner_init(&app.runner, &app.timers, RUNNER_TIMER_ID);

	app.hue = 0;
	app.sat = 255;
	app.val = 30;
	menu_init(&app.hsv_menu);
	hsv_component_entry_init(&hue_entry, "HUE", &app.hue, 0, 359, 1);
	hsv_component_entry_init(&sat_entry, "SAT", &app.sat, 0, 255, 1);
	hsv_component_entry_init(&val_entry, "VAL", &app.val, 0, 255, 1);
	screen_switch_entry_init(&back_entry, "BACK", &app.main_menu);
	menu_add_entry(&app.hsv_menu, &hue_entry.super);
	menu_add_entry(&app.hsv_menu, &sat_entry.super);
	menu_add_entry(&app.hsv_menu, &val_entry.super);
	menu_add_entry(&app.hsv_menu, &back_entry.super);

	app.active = &app.main_menu;

	bsp_neopixel_set_hsv(app.hue, app.sat, app.val);
	menu_draw(app.active);

	/* Objet actif : la file et la tâche sont créées par l'objet lui-même */
	app.events = xQueueCreate(APP_EVENT_QUEUE_LENGTH, sizeof(app_event_t));
	configASSERT(app.events != NULL);

	BaseType_t ok = xTaskCreate(app_task, "APP", APP_TASK_STACK_DEPTH,
			NULL, task_priority, &app.task);
	configASSERT(ok == pdPASS);

	shell_init(&app.shell);
	shell_add('l', sh_led, "Toggle LED");
	shell_add('e', sh_encoder, "Triggers the encoder");
	shell_add('n', sh_neopixel, "Changes Neopixel");
	shell_add('s', sh_switch, "Triggers a switch");
	bsp_vcp_receive();
}

/* Timeout 0 : on ne bloque jamais. Les callbacks des timers postent ici
 * depuis la tâche timer de FreeRTOS : s'ils bloquaient sur une file pleine,
 * ils bloqueraient tous les autres timers. */
bool app_post_event(app_event_t evt) {
	return xQueueSend(app.events, &evt, 0) == pdPASS;
}

bool app_post_event_from_isr(app_event_t evt) {
	BaseType_t woken = pdFALSE;
	BaseType_t ok = xQueueSendFromISR(app.events, &evt, &woken);
	portYIELD_FROM_ISR(woken);	/* bascule tout de suite si l'app est plus prioritaire */
	return ok == pdPASS;
}

/* Boucle de l'objet actif : le seul point de blocage est xQueueReceive */
static void app_task(void *unused) {
	(void)unused;	/* app est un singleton : pas besoin de passer l'objet */
	app_event_t evt;

	for (;;) {
		/* Plus de timers à scruter (ils postent EVT_TIMER depuis la tâche
		 * timer de FreeRTOS) : on peut attendre indéfiniment */
		xQueueReceive(app.events, &evt, portMAX_DELAY);
		app_handle_event(&evt);
	}
}

/* Run-to-completion : un handler va jusqu'au bout et ne bloque jamais */
static void app_handle_event(app_event_t *evt) {
	uint32_t now = bsp_get_tick_ms();

	if (app.mode == APP_MODE_TIME_BOMB) {
		if (time_bomb_handle_event(&app.bomb, evt, now)) {
			app.mode = APP_MODE_MENU;
		}
	} else if (app.mode == APP_MODE_RUNNER) {
		if (runner_handle_event(&app.runner, evt, now)) {
			app.mode = APP_MODE_MENU;
		}
	} else {
		menu_t *next = menu_handle_event(app.active, evt);

		if (next != NULL) {
			app.active = next;
			menu_enter(app.active);
		}
	}

	/* La bombe pilote le neopixel elle-meme (blanc a l'explosion) --
	 * ne pas l'ecraser avec HUE/SAT/VAL tant qu'elle a la main
	 * (idem pour le runner). */
	if (app.mode == APP_MODE_MENU) {
		bsp_neopixel_set_hsv(app.hue, app.sat, app.val);
		menu_draw(app.active);
	} else if (app.mode == APP_MODE_RUNNER) {
		runner_draw(&app.runner);
	} else {
		time_bomb_draw(&app.bomb);
	}

	if (evt->type == EVT_VCP_RX) {
		char chr = (char)evt->data;
		shell_process_char(&app.shell, chr);
		bsp_vcp_receive();
	}
}

static int sh_led(int argc, char ** argv) {
	if (argc != 2) {
		printf("Error: Arguments. Expected usage:\r\n");
		printf("%s <1,2,3>\r\n", argv[0]);
		return -1;
	}

	int index = atoi(argv[1]);

	if (index < 1 || index > 3) {
		printf("Error: LED Out of range. Expected usage:\r\n");
		printf("%s <1,2,3>\r\n", argv[0]);
		return -1;
	}

	index--;

	app.led_on[index] = !app.led_on[index];
	bsp_led_set(index, app.led_on[index]);

	menu_draw(app.active);

	return 0;
}

static int sh_encoder(int argc, char ** argv) {
	if (argc != 2) {
		printf("Error: Arguments. Expected usage:\r\n");
		printf("%s <u,d,p>\r\n", argv[0]);
		return -1;
	}

	switch (argv[1][0]) {
	case 'u':
		app_post_event((app_event_t){.type = EVT_ENCODER_ROTATE, .data = 1});
		return 0;
	case 'd':
		app_post_event((app_event_t){.type = EVT_ENCODER_ROTATE, .data = -1});
		return 0;
	case 'p':
		app_post_event((app_event_t){.type = EVT_ENCODER_CLICK});
		return 0;
	default:
		printf("Error: Command invalid. Expected usage:\r\n");
		printf("%s <u,d,p>\r\n", argv[0]);
		return -1;
	}
}

static int sh_neopixel(int argc, char ** argv) {
	printf("Not implemented yet\r\n");

	/* Example usage :
	 * n h 200 s 250 v 50 		# Configure everything
	 * n s 250 h 200 v 50		# Whatever the order
	 * n h 200 s 250			# Not everything is needed
	 * n						# Error
	 * n h						# Error
	 * n h 200 s 250 v 50 h150	# Error
	 */

	/* argc can be 3, 5 or 7
	 * Check 1 and 2, 3 and 4 then 5 and 6
	 */

	return 0;
}

static int sh_switch(int argc, char ** argv) {
	printf("Not implemented yet\r\n");
	return 0;
}
