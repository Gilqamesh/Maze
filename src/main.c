#include "defs.h"
#include "console/console.h"
#include "window/window.h"
#include "maze.h"

int WinMain(HINSTANCE app_handle, HINSTANCE prev_instance, LPSTR cmd_line, int show_cmd) {
    struct console console;

    if (!console__init_module(&console, 256)) {
        // ExitProcess(EXIT_FAILURE);
    }

    struct window window;

    window__init_module(&window, &console);

    struct maze maze;

    if (maze__create(&maze, v2u32(12, 3), v2u32(0, 0), v2u32(0, 0)) == false) {
        console__fatal(&console, "failed to create the maze");
    }

    for (u32 r = 0; r < maze.dims.y; ++r) {
        for (u32 c = 0; c < maze.dims.x; ++c) {
            console__log(&console, "%c", maze__get_entry(&maze, r, c));
        }
        console__log(&console, "\n");
    }

    window__create(&window, app_handle, "Maze generator", v2u32(400, 300), v2u32(800, 600));

    while (window__does_exist(&window)) {
        window__poll_inputs(&window);

        if (window__is_key_pressed(&window, KEY_ESCAPE)) {
            window__close_next_frame(&window);
        }

        window__draw_rectangle(&window, v2u32(0, 0), v2u32(300, 200), COLOR_WHITE);
        window__draw_rectangle(&window, v2u32(30, 30), v2u32(300, 200), COLOR_RED);

        window__end_draw(&window);
    }

    window__destroy(&window);

    window__deinit_module(&window);
    console__deinit_module(&console);
}
