#include "defs.h"
#include "console/console.h"
#include "window/window.h"
#include "maze.h"

static void maze__render(struct maze* self, struct window* window) {
    struct v2u32 entry_dims = v2u32(window->dims.x / self->dims.x, window->dims.y / self->dims.y);
    for (u32 r = 0; r < self->dims.y; ++r) {
        for (u32 c = 0; c < self->dims.x; ++c) {
            switch (maze__get_entry(self, v2u32(c, r))) {
                case MAZE_ENTRY_ROOM: {
                    struct v2u32 entry_p = v2u32(c * entry_dims.x, r * entry_dims.y);
                    window__draw_rectangle(window, entry_p, entry_dims, COLOR_RED);
                } break ;
                case MAZE_ENTRY_WALL: {
                    struct v2u32 entry_p = v2u32(c * entry_dims.x, r * entry_dims.y);
                    window__draw_rectangle(window, entry_p, entry_dims, COLOR_WHITE);
                } break ;
                default: {
                    console__fatal(window->console, "in 'maze__render': not supported maze entry");
                }
            }
        }
    }
}

static void maze__print(struct maze* self, struct console* console) {
    for (u32 r = 0; r < self->dims.y; ++r) {
        for (u32 c = 0; c < self->dims.x; ++c) {
            console__log(console, "%c", maze__get_entry(self, v2u32(c, r)));
        }
        console__log(console, "\n");
    }
}

int WinMain(HINSTANCE app_handle, HINSTANCE prev_instance, LPSTR cmd_line, int show_cmd) {
    struct console console;

    if (!console__init_module(&console, 256)) {
        // ExitProcess(EXIT_FAILURE);
    }

    struct window window;

    window__init_module(&window, &console);

    struct maze maze;

    if (maze__create(&maze, v2u32(12, 3), v2u32(1, 1), v2u32(0, 0), 42) == false) {
        console__fatal(&console, "failed to create the maze");
    }

    maze__build(&maze);

    // maze__print(&maze, &console);

    window__create(&window, app_handle, "Maze generator", v2u32(400, 300), v2u32(800, 600));

    while (window__does_exist(&window)) {
        window__poll_inputs(&window);

        if (window__is_key_pressed(&window, KEY_ESCAPE)) {
            window__close_next_frame(&window);
        }

        if (window__is_key_pressed(&window, KEY_RIGHT)) {
            maze__build_advance(&maze);
        }

        maze__render(&maze, &window);

        window__end_draw(&window);
    }

    window__destroy(&window);

    window__deinit_module(&window);
    console__deinit_module(&console);
}
