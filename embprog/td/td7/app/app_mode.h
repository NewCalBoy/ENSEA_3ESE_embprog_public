#ifndef APP_APP_MODE_H_
#define APP_APP_MODE_H_

/*
 * Mode au sommet du superloop : soit le systeme de menus (toute la
 * navigation habituelle), soit un ecran autonome qui prend la main sur
 * tout l'affichage et les boutons (APP_MODE_TIME_BOMB, APP_MODE_RUNNER).
 * APP_MODE_MENU reste la valeur par defaut.
 *
 * Fichier a part (plutot que dans app.h) pour que menu_entry/ puisse le
 * connaitre sans dependre d'app.h -- app.h depend deja de menu.h, qui
 * depend de menu_entry.h : un cycle sinon.
 */
typedef enum {
    APP_MODE_MENU,
    APP_MODE_TIME_BOMB,
    APP_MODE_RUNNER,
} app_mode_t;

#endif /* APP_APP_MODE_H_ */
