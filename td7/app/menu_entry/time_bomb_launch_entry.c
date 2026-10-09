#include "time_bomb_launch_entry.h"

void time_bomb_launch_entry_init(time_bomb_launch_entry_t *e, const char *label, app_mode_t *mode, time_bomb_t *bomb) {
	e->super.kind = MENU_ENTRY_TIME_BOMB_LAUNCH;
	e->super.label = label;
	e->mode = mode;
	e->bomb = bomb;
}

menu_t *time_bomb_launch_entry_activate(time_bomb_launch_entry_t *e, bsp_t *bsp) {
	*e->mode = APP_MODE_TIME_BOMB;
	time_bomb_enter(e->bomb, bsp);
	return NULL; /* ne navigue pas vers un autre menu_t */
}

void time_bomb_launch_entry_draw(const time_bomb_launch_entry_t *e, bsp_t *bsp, int y, bool selected) {
	menu_entry_draw_row(bsp, y, selected, false, e->super.label);
}
