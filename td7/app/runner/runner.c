#include "runner.h"

#define RUNNER_TICK_MS 40 /* 25 images par seconde */
#define GROUND_Y 56
#define SPEED 3

/* Generateur pseudo-aleatoire (LCG classique, Numerical Recipes) :
 * suffisant pour un jeu. Fourni, a ne pas modifier. */
uint32_t runner_rand(runner_t *r, uint32_t modulo) {
	r->rng = r->rng * 1664525u + 1013904223u;
	return (r->rng >> 16) % modulo;
}

static void runner_reset_game(runner_t *r, uint32_t seed) {
	dino_init(&r->dino);
	obstacle_init(&r->obs);
	r->ticks = 0;
	r->rng = seed;
	/* TODO : remettre a zero tout ce que vous ajouterez a runner_t
	 * (obstacles, score, vitesse...). */
}

void runner_init(runner_t *r, app_timers_t *timers, int timer_id) {
	r->state = RUNNER_READY;
	r->timers = timers;
	r->timer_id = timer_id;
	runner_reset_game(r, 1);
}

void runner_enter(runner_t *r, bsp_t *bsp) {
	r->state = RUNNER_READY;
	runner_reset_game(r, 1);
	app_timer_stop(r->timers, r->timer_id);
	bsp->neopixel_set_hsv(bsp, 120, 255, 40);
}

/* Un tick de jeu : tout ce qui bouge, bouge ici. */
static void runner_tick(runner_t *r) {
	r->ticks++;
	dino_update(&r->dino);

	// Etape 2 :
	if (!r->obs.active) {
		obstacle_spawn(&r->obs, 5, 15);
	}

	if (r->obs.active) {
		obstacle_update(&r->obs, SPEED);
	}

	// Etape 3 :
	if (obstacle_hits_dino(&r->obs,&r->dino))
	{
		r->state = RUNNER_GAME_OVER;
	}


	/* TODO etape 2 : faire defiler l'obstacle (et le faire reapparaitre).
	 * TODO etape 3 : detecter la collision -> GAME_OVER.
	 * TODO etape 4 : pool d'obstacles, generation aleatoire.
	 * TODO etape 5 : score, vitesse. */
}

static void runner_start(runner_t *r, bsp_t *bsp, uint32_t now_ms) {
	(void)bsp;
	/* Graine = instant de l'appui : imprevisible d'une partie a l'autre. */
	runner_reset_game(r, now_ms);
	r->state = RUNNER_PLAYING;
	r->next_tick_ms = now_ms + RUNNER_TICK_MS;
	app_timer_start(r->timers, r->timer_id, r->next_tick_ms);
}

bool runner_handle_event(runner_t *r, const app_event_t *evt, bsp_t *bsp, uint32_t now_ms) {
	if (evt->type == EVT_SW1_CLICK) {
		app_timer_stop(r->timers, r->timer_id);
		return true; /* retour au menu, quel que soit l'etat */
	}

	switch (r->state) {
	case RUNNER_READY:
		if (evt->type == EVT_SW2_CLICK) {
			runner_start(r, bsp, now_ms);
		}
		return false;

	case RUNNER_PLAYING:
		bsp->neopixel_set_hsv(bsp,240,255,40);
		if (evt->type == EVT_SW2_CLICK) {
			dino_jump(&r->dino); /* la garde est dans dino_jump */
		}
		else if (evt->type == EVT_TIMER && evt->data == r->timer_id) {
			/* Rearmement a partir de l'ancienne echeance (pas de
			 * now_ms) : un tick en retard n'allonge pas la periode. */
			r->next_tick_ms += RUNNER_TICK_MS;
			if ((int32_t)(now_ms - r->next_tick_ms) > 0) {
				r->next_tick_ms = now_ms + RUNNER_TICK_MS; /* trop en retard : on resynchronise */
			}
			app_timer_start(r->timers, r->timer_id, r->next_tick_ms);

			runner_tick(r);
		}
		return false;

	case RUNNER_GAME_OVER:
		bsp->neopixel_set_hsv(bsp,0,255,40);
		if (evt->type == EVT_SW2_CLICK)
		{
			runner_start(r, bsp, now_ms);

		}
	}
	return false;
}

void runner_draw(const runner_t *r, bsp_t *bsp) {
	bsp->oled_clear(bsp);
	switch (r->state){
	case RUNNER_READY:
		//	if (r->state == RUNNER_READY) {
		bsp->oled_draw_string(bsp, 4, 4, "RUNNER", true);
		bsp->oled_draw_string(bsp, 4, 28, "SW1 = BACK", true);
		bsp->oled_draw_string(bsp, 4, 44, "SW2 = START", true);
		bsp->oled_show(bsp);
		break;
		//		return;
		//	}

	case RUNNER_PLAYING:
		bsp->oled_fill_rect(bsp, 0, GROUND_Y, 128, 1, true);
		dino_draw(&r->dino, bsp, GROUND_Y, r->ticks);
		obstacle_draw(&r->obs, bsp, GROUND_Y);
		bsp->oled_show(bsp);
		break;

	case RUNNER_GAME_OVER:
		bsp->oled_draw_string(bsp, 36, 8, "GAME OVER", true);
		bsp->oled_show(bsp);
		break;

		/* TODO : dessiner obstacles, score, ecran GAME OVER... */
	}

}
