#include "runner_launch_entry.h"

void runner_launch_entry_init(runner_launch_entry_t *e, const char *label, app_mode_t *mode, runner_t *runner) {
	e->super.kind = MENU_ENTRY_RUNNER_LAUNCH;
	e->super.label = label;
	e->mode = mode;
	e->runner = runner;
}

menu_t *runner_launch_entry_activate(runner_launch_entry_t *e, bsp_t *bsp) {
	*e->mode = APP_MODE_RUNNER;
	runner_enter(e->runner, bsp);
	return NULL; /* ne navigue pas vers un autre menu_t */
}

void runner_launch_entry_draw(const runner_launch_entry_t *e, bsp_t *bsp, int y, bool selected) {
	menu_entry_draw_row(bsp, y, selected, false, e->super.label);
}
