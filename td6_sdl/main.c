#include "app.h"
#include "bsp/bsp.h"

int main(void) {
    bsp_t bsp;
    app_t app;

    if (!bsp_init(&bsp, &app.events)) {
        return 1;
    }

    app_init(&app, &bsp);

    while (bsp_process(&bsp)) {
        app_process(&app);
    }

    bsp_deinit(&bsp);
    return 0;
}
