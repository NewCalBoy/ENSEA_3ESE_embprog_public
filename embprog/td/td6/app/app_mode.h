#ifndef APP_APP_MODE_H_
#define APP_APP_MODE_H_

/*
 * Mode au sommet du superloop : soit le systeme de menus (toute la
 * navigation habituelle), soit un ecran autonome qui prend la main sur
 * tout l'affichage et les boutons. APP_MODE_TIME_BOMB est le premier ;
 * APP_MODE_MENU reste la valeur par defaut -- une 3e valeur est prevue
 * pour un futur TD en autonomie.
 *
 * Fichier a part (plutot que dans app.h) pour que menu_entry/ puisse le
 * connaitre sans dependre d'app.h -- app.h depend deja de menu.h, qui
 * depend de menu_entry.h : un cycle sinon.
 */
typedef enum {
    APP_MODE_MENU,
    APP_MODE_TIME_BOMB,
} app_mode_t;

#endif /* APP_APP_MODE_H_ */
