#include "dino.h"

#define DINO_JUMP_SPEED 8/* px/tick initiaux : pic a 6+5+4+3+2+1 = 21 px */
#define DINO_GRAVITY 1    /* px/tick perdus a chaque tick */

void dino_init(dino_t *d) {
	d->state = DINO_RUNNING;
	d->height = 0;
	d->vy = 0;
}

bool dino_jump(dino_t *d) {
	if (d->state != DINO_RUNNING) {
		return false; /* garde : pas de saut en l'air */
	}
	d->state = DINO_JUMPING;
	d->vy = DINO_JUMP_SPEED;
	return true;
}

void dino_update(dino_t *d) {
	if (d->state != DINO_JUMPING) {
		return;
	}

	d->height += d->vy;
	d->vy -= DINO_GRAVITY;

	if (d->height <= 0) { /* garde : retombe au sol */
		d->height = 0;
		d->vy = 0;
		d->state = DINO_RUNNING;
	}
}

void dino_draw(const dino_t *d, bsp_t *bsp, int ground_y, int frame) {
	(void)frame; /* TODO etape 1 : a utiliser pour animer les pattes */

	int top = ground_y - DINO_H - d->height;
	bsp->oled_fill_rect(bsp, DINO_X, top, DINO_W, DINO_H, true);
	bsp->oled_fill_rect(bsp, DINO_X+5, top+2, EYE_W, EYE_H, false);
	bsp->oled_fill_rect(bsp, DINO_X-TAIL_W, top+6, TAIL_W, TAIL_H, true);
	bsp->oled_fill_rect(bsp, DINO_X-TAIL_W, top+7-TAIL_H, 2, 2, true);
	bsp->oled_fill_rect(bsp, DINO_X, top+DINO_H-LEG_H,DINO_W, LEG_H, false);
	if ((frame / 2) % 2)
	{
		bsp->oled_fill_rect(bsp, DINO_X+LEG_X,top+DINO_H-LEG_H,LEG_W,LEG_H, true);
	}
	else
	{
		bsp->oled_fill_rect(bsp, DINO_X+LEG_X+5,top+DINO_H-LEG_H,LEG_W,LEG_H, true);
	}

	if (d->state==DINO_JUMPING)
	{
		bsp->oled_fill_rect(bsp, DINO_X+LEG_X,top+DINO_H-LEG_H,LEG_W,LEG_H, true);
		bsp->oled_fill_rect(bsp, DINO_X+LEG_X+5,top+DINO_H-LEG_H,LEG_W,LEG_H, true);
	}

	/* TODO etape 1 : remplacer ce rectangle par un vrai personnage
	 * (corps, oeil, pattes qui alternent en courant, pattes jointes en
	 * l'air...). Le dino occupe la zone DINO_W x DINO_H a partir de
	 * (DINO_X, top) : restez dedans, la collision (etape 3) en depend. */

}
