#include "runner_launch_entry.h"

void runner_launch_entry_init(runner_launch_entry_t *e, const char *label, app_mode_t *mode, runner_t *runner) {
	e->super.kind = MENU_ENTRY_RUNNER_LAUNCH;
	e->super.label = label;
	e->mode = mode;
	e->runner = runner;
}

menu_t *runner_launch_entry_activate(runner_launch_entry_t *e) {
	*e->mode = APP_MODE_RUNNER;
	runner_enter(e->runner);
	return NULL; /* ne navigue pas vers un autre menu_t */
}

void runner_launch_entry_draw(const runner_launch_entry_t *e, int y, bool selected) {
	menu_entry_draw_row(y, selected, false, e->super.label);
}
