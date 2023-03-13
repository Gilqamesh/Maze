#include "defs.h"
#include "console/console.h"
#include "window/window.h"

int WinMain(HINSTANCE app_handle, HINSTANCE prev_instance, LPSTR cmd_line, int show_cmd) {
    struct console console;

    if (!console__init_module(&console, 256)) {
        // ExitProcess(EXIT_FAILURE);
    }

    struct window window;

    window__init_module(&window, &console);

    struct v2u32 window_p = v2u32(400, 300);
    struct v2u32 window_d = v2u32(800, 600);
    window__create(&window, app_handle, "Maze generator", window_p, window_d);

    while (window__does_exist(&window)) {
        window__poll_inputs(&window);

        window__end_draw(&window);
    }

    window__deinit_module(&window);

    console__deinit_module(&console);
}
