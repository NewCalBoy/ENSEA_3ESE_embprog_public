#ifndef APP_RUNNER_OBSTACLE_H_
#define APP_RUNNER_OBSTACLE_H_

#include <stdbool.h>

#include "bsp.h"
#include "dino.h"

/*
 * OBSTACLE -- un cactus : rectangle pose au sol qui defile vers la
 * gauche. Les obstacles vivent dans un pool de taille fixe (pas de
 * malloc en embarque) : `active` dit si l'emplacement est occupe.
 */

typedef struct {
    bool active;
    int x; /* bord gauche, en px ecran (peut etre > 127 : pas encore visible) */
    int w;
    int h;
} obstacle_t;

void obstacle_init(obstacle_t *o);

/* Active l'obstacle en bordure droite de l'ecran. */
void obstacle_spawn(obstacle_t *o, int w, int h);

/* Fait defiler de `speed` px ; se desactive des qu'il sort a gauche. */
void obstacle_update(obstacle_t *o, int speed);

/* Garde de collision : recouvrement de rectangles (AABB) avec le dino. */
bool obstacle_hits_dino(const obstacle_t *o, const dino_t *d);

void obstacle_draw(const obstacle_t *o, bsp_t *bsp, int ground_y);

#endif /* APP_RUNNER_OBSTACLE_H_ */
