#include "defs.h"
#include "console/console.h"
#include "window/window.h"
#include "maze.h"
#include "readers/bmp_loader.h"
#include "ui/button.h"

static void maze__render(struct maze* self, struct window* window) {
    struct v2r32 entry_dims = v2r32(round((r32)window->dims.x / (r32)self->dims.x), round((r32)window->dims.y / (r32)self->dims.y));
    for (u32 r = 0; r < self->dims.y; ++r) {
        for (u32 c = 0; c < self->dims.x; ++c) {
            switch (maze__get_entry(self, v2u32(c, r))) {
                case MAZE_ENTRY_ROOM: {
                    struct v2r32 entry_p = v2r32(c * entry_dims.x, r * entry_dims.y);
                    window__draw_rectangle(window, entry_p, entry_dims, COLOR_RED);
                } break ;
                case MAZE_ENTRY_WALL: {
                    struct v2r32 entry_p = v2r32(c * entry_dims.x, r * entry_dims.y);
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

// TODO(david): asset loader
static void assets__load(struct bitmap* bitmap_generate, struct console* console) {
    struct file_reader reader;

    struct bmp_loader bmp_loader;

    const char* filename = "assets/art.bmp";
    file_reader__create(&reader, console, filename);

    if (bmp_loader__create(&bmp_loader, &reader, bitmap_generate) == false) {
        console__fatal(console, "in 'assets__load': couldn't load file: %s\n", filename);
    }

    file_reader__destroy(&reader);
}

int WinMain(HINSTANCE app_handle, HINSTANCE prev_instance, LPSTR cmd_line, int show_cmd) {
    struct console console;

    if (!console__init_module(&console, 512)) {
        // ExitProcess(EXIT_FAILURE);
    }

    struct window window;

    window__init_module(&window, &console);

    struct maze maze;

    struct v2u32 maze_dims = v2u32(12, 3);
    if (maze__create(&maze, maze_dims, v2u32(1, 1), v2u32(0, 0), 42) == false) {
        console__fatal(&console, "failed to create the maze");
    }

    maze__build(&maze);

    window__create(&window, app_handle, "Maze generator", v2u32(400, 300), v2u32(800, 600));

    struct button generate_new_maze_button;

    button__create(
        &generate_new_maze_button, &window,
        v2r32(window.dims.x / 100, window.dims.y / 100), v2r32(window.dims.x / 10, window.dims.y / 10),
        COLOR_BLUE, COLOR_YELLOW, COLOR_RED
    );

    while (window__does_exist(&window)) {
        window__poll_inputs(&window);

        if (window__is_key_pressed(&window, KEY_ESCAPE)) {
            window__close_next_frame(&window);
        }

        if (window__is_key_pressed(&window, KEY_RIGHT)) {
            maze__build_advance(&maze);
        }

        maze__render(&maze, &window);


        button__update(&generate_new_maze_button);
        if (button__is_pressed(&generate_new_maze_button)) {
            maze__destroy(&maze);
            ++maze_dims.x;
            ++maze_dims.y;
            if (maze__create(&maze, maze_dims, v2u32(1, 1), v2u32(0, 0), 100) == false) {
                console__fatal(&console, "failed to create the maze");
            }
            maze__build(&maze);
        }
        button__render(&generate_new_maze_button);

        window__end_draw(&window);
    }

    window__destroy(&window);
    window__deinit_module(&window);
    console__deinit_module(&console);
}
