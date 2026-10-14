#ifndef APP_MENU_ENTRY_RUNNER_LAUNCH_ENTRY_H_
#define APP_MENU_ENTRY_RUNNER_LAUNCH_ENTRY_H_

#include "menu_entry.h"
#include "app_mode.h"
#include "runner.h"

/*
 * CONCRETE ENTRY: contrairement a screen_switch_entry, ne navigue pas
 * vers un autre menu_t -- elle bascule app_mode_t pour donner la main a
 * l'ecran du runner, qui n'est pas un menu (voir app_mode.h).
 */
typedef struct {
	menu_entry_t super; /* inherited menu_entry_t */

	app_mode_t *mode;
	runner_t *runner;
} runner_launch_entry_t;

void runner_launch_entry_init(runner_launch_entry_t *e, const char *label, app_mode_t *mode, runner_t *runner);

menu_t *runner_launch_entry_activate(runner_launch_entry_t *e, bsp_t *bsp);
void runner_launch_entry_draw(const runner_launch_entry_t *e, bsp_t *bsp, int y, bool selected);

#endif /* APP_MENU_ENTRY_RUNNER_LAUNCH_ENTRY_H_ */
