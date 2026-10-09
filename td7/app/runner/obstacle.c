#include "obstacle.h"

#define SCREEN_W 128

/* TODO etape 2 : implementer les 4 fonctions ci-dessous (voir obstacle.h
 * pour le contrat de chacune). */

void obstacle_init(obstacle_t *o) {
    (void)o;
}

void obstacle_spawn(obstacle_t *o, int w, int h) {
    (void)o;
    (void)w;
    (void)h;
}

void obstacle_update(obstacle_t *o, int speed) {
    (void)o;
    (void)speed;
}

void obstacle_draw(const obstacle_t *o, bsp_t *bsp, int ground_y) {
    (void)o;
    (void)bsp;
    (void)ground_y;
}

/* TODO etape 3 : garde de collision. */
bool obstacle_hits_dino(const obstacle_t *o, const dino_t *d) {
    (void)o;
    (void)d;
    return false;
}
