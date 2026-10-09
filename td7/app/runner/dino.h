#ifndef APP_RUNNER_DINO_H_
#define APP_RUNNER_DINO_H_

#include <stdbool.h>

#include "bsp.h"

/*
 * DINO -- le personnage du runner, une petite "classe" a part entiere :
 * ses donnees (etat, hauteur, vitesse verticale) et ses operations
 * (init, jump, update, draw) vivent ici, runner.c n'y touche jamais
 * directement.
 *
 * Sous-FSM a 2 etats, independante de celle du jeu :
 *
 *   RUNNING --(jump, garde : etat == RUNNING)--> JUMPING
 *   JUMPING --(update, garde : height <= 0)----> RUNNING
 *
 * La garde de jump() est ce qui interdit le double saut en l'air.
 *
 * `height` est la hauteur au-dessus du sol en pixels (0 = pose au sol,
 * l'axe vers le HAUT -- contrairement a l'OLED dont y croit vers le bas).
 */

#define DINO_X 12
#define DINO_W 8
#define DINO_H 10

typedef enum {
    DINO_RUNNING,
    DINO_JUMPING,
} dino_state_t;

typedef struct {
    dino_state_t state;
    int height; /* px au-dessus du sol */
    int vy;     /* px par tick, positif = monte */
} dino_t;

void dino_init(dino_t *d);

/* Declenche un saut si (et seulement si) le dino est au sol.
 * Renvoie true si le saut a bien ete pris en compte. */
bool dino_jump(dino_t *d);

/* A appeler une fois par tick de jeu : applique la gravite. */
void dino_update(dino_t *d);

/* `frame` : compteur de ticks, sert uniquement a alterner les pattes. */
void dino_draw(const dino_t *d, bsp_t *bsp, int ground_y, int frame);

#endif /* APP_RUNNER_DINO_H_ */
