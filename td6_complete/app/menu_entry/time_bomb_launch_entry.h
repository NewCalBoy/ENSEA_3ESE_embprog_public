#ifndef APP_MENU_ENTRY_TIME_BOMB_LAUNCH_ENTRY_H_
#define APP_MENU_ENTRY_TIME_BOMB_LAUNCH_ENTRY_H_

#include "menu_entry.h"
#include "app_mode.h"
#include "time_bomb.h"

/*
 * CONCRETE ENTRY: contrairement a screen_switch_entry, ne navigue pas
 * vers un autre menu_t -- elle bascule app_mode_t pour donner la main a
 * l'ecran TIME BOMB, qui n'est pas un menu (voir app_mode.h).
 */
typedef struct {
	menu_entry_t super; /* inherited menu_entry_t */

	app_mode_t *mode;
	time_bomb_t *bomb;
} time_bomb_launch_entry_t;

void time_bomb_launch_entry_init(time_bomb_launch_entry_t *e, const char *label, app_mode_t *mode, time_bomb_t *bomb);

menu_t *time_bomb_launch_entry_activate(time_bomb_launch_entry_t *e, bsp_t *bsp);
void time_bomb_launch_entry_draw(const time_bomb_launch_entry_t *e, bsp_t *bsp, int y, bool selected);

#endif /* APP_MENU_ENTRY_TIME_BOMB_LAUNCH_ENTRY_H_ */
