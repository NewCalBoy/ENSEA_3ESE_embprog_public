#include "bsp.h"

#include <SDL3/SDL.h>

#include "buttons.h"
#include "display.h"
#include "encoder.h"
#include "led.h"
#include "neopixel.h"

#define WINDOW_W 720
#define WINDOW_H 375

/* Etat prive du simulateur : invisible pour l'app, qui ne voit que les
 * champs de bsp_t (bsp.h). Singleton statique, comme bsp_ctx_t cote
 * materiel (td5_complete/bsp/bsp.c) -- ctx pointe dessus. */
typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool quit_requested;

    event_queue_t *events;

    bsp_led_t led;
    bsp_neopixel_t neopixel;
    bsp_display_t display;
    bsp_encoder_t encoder;
    bsp_buttons_t buttons;
} bsp_priv_t;

static bsp_priv_t bsp_priv;

static void render(bsp_priv_t *priv) {
    SDL_SetRenderDrawColor(priv->renderer, 25, 26, 30, 255);
    SDL_RenderClear(priv->renderer);

    bsp_led_render(&priv->led, priv->renderer);
    bsp_neopixel_render(&priv->neopixel, priv->renderer);
    bsp_display_render(&priv->display, priv->renderer);
    bsp_encoder_render(&priv->encoder, priv->renderer);
    bsp_buttons_render(&priv->buttons, priv->renderer);

    SDL_SetRenderDrawColor(priv->renderer, 150, 150, 155, 255);
    SDL_RenderDebugText(priv->renderer, 30, 350,
                        "UP/DOWN: encodeur tourne   RETURN: appui encodeur   A/Z: boutons poussoirs SW1/SW2");

    SDL_RenderPresent(priv->renderer);
}

/* --- API exposee a l'application (voir bsp.h) --------------------------- */

static void bsp_led_set_impl(bsp_t *bsp, uint8_t led, bool on) {
    bsp_priv_t *priv = (bsp_priv_t *)bsp->ctx;
    bsp_led_write(&priv->led, led, on);
}

static void bsp_neopixel_set_hsv_impl(bsp_t *bsp, int hue, int sat, int val) {
    bsp_priv_t *priv = (bsp_priv_t *)bsp->ctx;
    bsp_neopixel_write(&priv->neopixel, hue, sat, val);
}

static bool bsp_sw1_pressed_impl(bsp_t *bsp) {
    bsp_priv_t *priv = (bsp_priv_t *)bsp->ctx;
    return bsp_buttons_take_sw1(&priv->buttons);
}

static bool bsp_sw2_pressed_impl(bsp_t *bsp) {
    bsp_priv_t *priv = (bsp_priv_t *)bsp->ctx;
    return bsp_buttons_take_sw2(&priv->buttons);
}

static uint32_t bsp_get_tick_ms_impl(bsp_t *bsp) {
    (void)bsp;
    return (uint32_t)SDL_GetTicks();
}

static int32_t bsp_encoder_get_delta_impl(bsp_t *bsp) {
    bsp_priv_t *priv = (bsp_priv_t *)bsp->ctx;
    return bsp_encoder_take_delta(&priv->encoder);
}

static bool bsp_enc_pressed_impl(bsp_t *bsp) {
    bsp_priv_t *priv = (bsp_priv_t *)bsp->ctx;
    return bsp_encoder_take_button(&priv->encoder);
}

static void bsp_oled_clear_impl(bsp_t *bsp) {
    bsp_priv_t *priv = (bsp_priv_t *)bsp->ctx;
    bsp_display_clear(&priv->display);
}

static void bsp_oled_fill_rect_impl(bsp_t *bsp, int x, int y, int w, int h, bool on) {
    bsp_priv_t *priv = (bsp_priv_t *)bsp->ctx;
    bsp_display_fill(&priv->display, x, y, w, h, on);
}

static void bsp_oled_draw_string_impl(bsp_t *bsp, int x, int y, const char *str, bool on) {
    bsp_priv_t *priv = (bsp_priv_t *)bsp->ctx;
    bsp_display_text(&priv->display, x, y, str, on);
}

static void bsp_oled_show_impl(bsp_t *bsp) {
    bsp_priv_t *priv = (bsp_priv_t *)bsp->ctx;
    render(priv);
}

/* --- Cycle de vie (reserve a main.c) ------------------------------------ */

bool bsp_init(bsp_t *bsp, event_queue_t *events) {
    bsp_priv_t *priv = &bsp_priv;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return false;
    }

    priv->window = SDL_CreateWindow("BSP simulator - td5 (event-driven)", WINDOW_W, WINDOW_H, 0);
    if (!priv->window) {
        SDL_Quit();
        return false;
    }

    priv->renderer = SDL_CreateRenderer(priv->window, NULL);
    if (!priv->renderer) {
        SDL_DestroyWindow(priv->window);
        SDL_Quit();
        return false;
    }
    SDL_SetRenderVSync(priv->renderer, 1);

    priv->quit_requested = false;
    priv->events = events;

    bsp_led_init(&priv->led);
    bsp_neopixel_init(&priv->neopixel);
    bsp_display_init(&priv->display);
    bsp_encoder_init(&priv->encoder);
    bsp_buttons_init(&priv->buttons);

    bsp->ctx = priv;
    bsp->led_set = bsp_led_set_impl;
    bsp->neopixel_set_hsv = bsp_neopixel_set_hsv_impl;
    bsp->sw1_pressed = bsp_sw1_pressed_impl;
    bsp->sw2_pressed = bsp_sw2_pressed_impl;
    bsp->get_tick_ms = bsp_get_tick_ms_impl;
    bsp->encoder_get_delta = bsp_encoder_get_delta_impl;
    bsp->enc_pressed = bsp_enc_pressed_impl;
    bsp->oled_clear = bsp_oled_clear_impl;
    bsp->oled_fill_rect = bsp_oled_fill_rect_impl;
    bsp->oled_draw_string = bsp_oled_draw_string_impl;
    bsp->oled_show = bsp_oled_show_impl;

    render(priv);
    return true;
}

void bsp_deinit(bsp_t *bsp) {
    bsp_priv_t *priv = (bsp_priv_t *)bsp->ctx;

    if (priv->renderer) {
        SDL_DestroyRenderer(priv->renderer);
        priv->renderer = NULL;
    }
    if (priv->window) {
        SDL_DestroyWindow(priv->window);
        priv->window = NULL;
    }
    SDL_Quit();
}

bool bsp_process(bsp_t *bsp) {
    bsp_priv_t *priv = (bsp_priv_t *)bsp->ctx;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                priv->quit_requested = true;
                break;
            case SDL_EVENT_KEY_DOWN:
                bsp_encoder_on_key(&priv->encoder, event.key.key, true, event.key.repeat);
                bsp_buttons_on_key(&priv->buttons, event.key.key, true, event.key.repeat);
                break;
            case SDL_EVENT_KEY_UP:
                bsp_encoder_on_key(&priv->encoder, event.key.key, false, false);
                bsp_buttons_on_key(&priv->buttons, event.key.key, false, false);
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    bsp_encoder_on_click(&priv->encoder, event.button.x, event.button.y, true);
                    bsp_buttons_on_click(&priv->buttons, event.button.x, event.button.y, true);
                }
                break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    bsp_encoder_on_click(&priv->encoder, event.button.x, event.button.y, false);
                    bsp_buttons_on_click(&priv->buttons, event.button.x, event.button.y, false);
                }
                break;
            default:
                break;
        }
    }

    /*
     * Point de conversion "front detecte" -> "evenement pousse" : cote
     * materiel (td5_complete/bsp/bsp.c), c'est HAL_GPIO_EXTI_Callback qui
     * joue ce role pour les clicks, bsp_process pour la rotation. Ici,
     * pas d'interruption : bsp_process pousse tout, une fois par frame,
     * dans la queue recue a bsp_init -- meme `events` que celui d'app_t.
     */
    int32_t delta = bsp_encoder_take_delta(&priv->encoder);
    if (delta != 0) {
        event_queue_push(priv->events, (app_event_t){.type = EVT_ENCODER_ROTATE, .data = delta});
    }
    if (bsp_encoder_take_button(&priv->encoder)) {
        event_queue_push(priv->events, (app_event_t){.type = EVT_ENCODER_CLICK});
    }
    if (bsp_buttons_take_sw1(&priv->buttons)) {
        event_queue_push(priv->events, (app_event_t){.type = EVT_SW1_CLICK});
    }
    if (bsp_buttons_take_sw2(&priv->buttons)) {
        event_queue_push(priv->events, (app_event_t){.type = EVT_SW2_CLICK});
    }

    render(priv);

    return !priv->quit_requested;
}
