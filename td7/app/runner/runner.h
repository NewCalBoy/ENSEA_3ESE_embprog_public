#ifndef APP_RUNNER_RUNNER_H_
#define APP_RUNNER_RUNNER_H_

#include <stdbool.h>
#include <stdint.h>

#include "bsp.h"
#include "events.h"
#include "timers.h"
#include "dino.h"
#include "obstacle.h"

/*
 * RUNNER -- un "trex runner" : le dino court, SW2 le fait sauter par
 * dessus les cactus. Meme principe d'integration que time_bomb : pas un
 * menu_t, un ecran autonome (voir app_mode_t / runner_launch_entry).
 *
 * FSM du jeu (etat de depart du TD) :
 *
 *   READY --(SW2)--> PLAYING --(timer)--> PLAYING   (un tick de jeu)
 *
 *   SW1 : retour au menu depuis n'importe quel etat.
 *
 * Le jeu avance au rythme d'un timer periodique (RUNNER_TICK_MS) : toute
 * la logique est dans la reponse a EVT_TIMER, comme pour le clignotement
 * de la bombe. Pas de delai bloquant, pas de lecture directe des boutons.
 *
 * A vous de completer cette structure au fil des etapes : obstacles,
 * score, vitesse... (ajoutez vos champs ici).
 */

typedef enum {
    RUNNER_READY,
    RUNNER_PLAYING,
    /* TODO etape 3 : RUNNER_GAME_OVER */
} runner_state_t;

typedef struct {
    runner_state_t state;

    dino_t dino;
    int ticks;  /* ticks ecoules depuis le debut de la partie */
    uint32_t rng; /* etat du generateur pseudo-aleatoire (voir runner_rand) */

    uint32_t next_tick_ms; /* echeance du prochain tick (sans derive) */

    app_timers_t *timers;
    int timer_id;
} runner_t;

void runner_init(runner_t *r, app_timers_t *timers, int timer_id);

/* (Re)entre sur l'ecran : repart de RUNNER_READY, timer desarme. */
void runner_enter(runner_t *r, bsp_t *bsp);

/* Renvoie true si cet evenement doit faire quitter l'ecran (retour au
 * menu principal) -- a l'appelant (app_process) de rebasculer le mode. */
bool runner_handle_event(runner_t *r, const app_event_t *evt, bsp_t *bsp, uint32_t now_ms);

void runner_draw(const runner_t *r, bsp_t *bsp);

/* Nombre pseudo-aleatoire dans [0, modulo[. Fournie : a utiliser a
 * l'etape 4 (taille et espacement des cactus). */
uint32_t runner_rand(runner_t *r, uint32_t modulo);

#endif /* APP_RUNNER_RUNNER_H_ */
