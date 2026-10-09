#include "obstacle.h"

#define SCREEN_W 128

/* TODO etape 2 : implementer les 4 fonctions ci-dessous (voir obstacle.h
 * pour le contrat de chacune). */

void obstacle_init(obstacle_t *o) {
	o->active=false;
	o->x=0;
	o->h=0;
	o->w=0;
}

void obstacle_spawn(obstacle_t *o, int w, int h) {
	o->active=true;
	o->x=127;
	o->h=h;
	o->w=w;
}

void obstacle_update(obstacle_t *o, int speed) {
	o->x-=speed;
}

void obstacle_draw(const obstacle_t *o, bsp_t *bsp, int ground_y) {
	int top = ground_y - o->h;
	bsp->oled_fill_rect(bsp, o->x, top, o->w, o->h, true);
}

/* TODO etape 3 : garde de collision. */
bool obstacle_hits_dino(const obstacle_t *o, const dino_t *d) {
    (void)o;
    (void)d;
    return false;
}
