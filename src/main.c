#include "defs.h"
#include "console/console.h"
#include "window/window.h"
#include "maze.h"
#include "readers/bmp_loader.h"
#include "ui/ui_group.h"

static void maze__render(struct maze* self, struct window* window, struct ui_group* render_context_holder, struct box* render_context) {
    struct v2r32 entry_offset = v2r32(render_context_holder->p.x + render_context->p.x, render_context_holder->p.y + render_context->p.y);
    struct v2r32 entry_dims   = v2r32((r32) render_context->dims.x / self->dims.x, (r32) render_context->dims.y / self->dims.y);;
    // struct v2r32 entry_dims = v2r32((r32)window->dims.x / (r32)self->dims.x, (r32)window->dims.y / (r32)self->dims.y);
    for (u32 r = 0; r < self->dims.y; ++r) {
        for (u32 c = 0; c < self->dims.x; ++c) {
            switch (maze__get_entry(self, v2u32(c, r))) {
                case MAZE_ENTRY_ROOM: {
                    struct v2r32 entry_p = v2r32(c * entry_dims.x + entry_offset.x, r * entry_dims.y + entry_offset.y);
                    window__draw_rectangle(window, entry_p, entry_dims, COLOR_RED);
                } break ;
                case MAZE_ENTRY_WALL: {
                    struct v2r32 entry_p = v2r32(c * entry_dims.x + entry_offset.x, r * entry_dims.y + entry_offset.y);
                    window__draw_rectangle(window, entry_p, entry_dims, COLOR_BLACK);
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
    if (maze__create(&maze, maze_dims, 42) == false) {
        console__fatal(&console, "failed to create the maze");
    }

    maze__build(&maze);

    window__create(&window, app_handle, "Maze generator", v2u32(400, 300), v2u32(800, 600));

    // NOTE(david): for the two ui groups, overlay and main canvas
    r32 y_arbitrary_split_for_the_two_groups = (r32) window.dims.y / 5.0f;
    r32 y_padding_between_the_two_groups     = (r32) window.dims.y / 100.0f;
    struct v2r32 offset_for_two_ui_groups    = v2r32((r32) window.dims.x / 100.0f, (r32) window.dims.y / 100.0f);

    struct ui_group ui_group_main_canvas;
    struct v2r32 ui_group_main_canvas_p    = v2r32(offset_for_two_ui_groups.x, y_arbitrary_split_for_the_two_groups + y_padding_between_the_two_groups);
    struct v2r32 ui_group_main_canvas_dims = v2r32(
        (r32) window.dims.x - 2.0f * offset_for_two_ui_groups.x,
        (r32) window.dims.y - y_arbitrary_split_for_the_two_groups - y_padding_between_the_two_groups - offset_for_two_ui_groups.y
    );
    ui_group__create(&ui_group_main_canvas, &window, ui_group_main_canvas_p, ui_group_main_canvas_dims, COLOR_WHITE, COLOR_GRAY);

    struct box maze_holder_box;
    struct v2r32 maze_holder_box_p    = v2r32(ui_group_main_canvas_dims.x / 100.0f, ui_group_main_canvas_dims.y / 100.0f);
    struct v2r32 maze_holder_box_dims = v2r32(
        ui_group_main_canvas_dims.x - 2.0f * ui_group_main_canvas_dims.x / 100.0f,
        ui_group_main_canvas_dims.y - 2.0f * ui_group_main_canvas_dims.y / 100.0f
    );
    box__create(&maze_holder_box, maze_holder_box_p, maze_holder_box_dims, COLOR_BLACK);
    ui_group__push_box(&ui_group_main_canvas, &maze_holder_box);

    struct ui_group ui_group_overlay;
    struct v2r32 ui_group_overlay_p    = v2r32(offset_for_two_ui_groups.x, offset_for_two_ui_groups.y);
    struct v2r32 ui_group_overlay_dims = v2r32(
        (r32) window.dims.x - 2.0f * offset_for_two_ui_groups.x,
        y_arbitrary_split_for_the_two_groups - offset_for_two_ui_groups.y - y_padding_between_the_two_groups
    );
    ui_group__create(&ui_group_overlay, &window, ui_group_overlay_p, ui_group_overlay_dims, COLOR_WHITE, COLOR_GRAY);

    struct button generate_new_maze_button;
    struct v2r32 generate_new_maze_button_p    = v2r32(0.0f, 0.0f);
    struct v2r32 generate_new_maze_button_dims = v2r32(50.0f, 50.0f);
    button__create(&generate_new_maze_button, generate_new_maze_button_p, generate_new_maze_button_dims, COLOR_BLUE, COLOR_YELLOW, COLOR_RED);
    ui_group__push_button(&ui_group_overlay, &generate_new_maze_button);

    while (window__does_exist(&window)) {
        window__poll_inputs(&window);

        if (window__is_key_pressed(&window, KEY_ESCAPE)) {
            window__close_next_frame(&window);
        }

        if (window__is_key_pressed(&window, KEY_RIGHT)) {
            maze__build_advance(&maze);
        }

        ui_group__update_and_render(&ui_group_overlay);
        ui_group__update_and_render(&ui_group_main_canvas);
        maze__render(&maze, &window, &ui_group_main_canvas, &maze_holder_box);

        if (button__is_pressed(&generate_new_maze_button)) {
            maze__destroy(&maze);
            ++maze_dims.x;
            ++maze_dims.y;
            if (maze__create(&maze, maze_dims, 100) == false) {
                console__fatal(&console, "failed to create the maze");
            }
            maze__build(&maze);
        }

        window__end_draw(&window);
    }

    ui_group__destroy(&ui_group_overlay);
    window__destroy(&window);
    window__deinit_module(&window);
    console__deinit_module(&console);
}
