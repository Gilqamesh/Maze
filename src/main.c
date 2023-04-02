#include "defs.h"
#include "console/console.h"
#include "window/window.h"
#include "maze.h"
#include "reader/bmp_loader.h"
#include "ui/ui_group.h"

static void maze__render(struct maze* self, struct window* window, struct ui_group* render_context_holder, struct box* render_context) {
    struct v2r32 ui_group_scale = v2r32(
        (r32) window->dims.x / (r32) render_context_holder->original_window_dims.x,
        (r32) window->dims.y / (r32) render_context_holder->original_window_dims.y
    );
    struct v2r32 entry_offset = v2r32(
        ui_group_scale.x * render_context->p.x + ui_group_scale.x * render_context_holder->p.x,
        ui_group_scale.y * render_context->p.y + ui_group_scale.y * render_context_holder->p.y
    );
    struct v2r32 entry_dims   = v2r32(
        ui_group_scale.x * render_context->dims.x / (r32) self->dims.x,
        ui_group_scale.y * render_context->dims.y / (r32) self->dims.y
    );
    for (u32 r = 0; r < self->dims.y; ++r) {
        for (u32 c = 0; c < self->dims.x; ++c) {
            switch (maze__get_entry(self, v2u16(c, r))) {
                case MAZE_ENTRY_ROOM: {
                    struct v2r32 entry_p = v2r32(c * entry_dims.x + entry_offset.x, r * entry_dims.y + entry_offset.y);
                    window__draw_rectangle(window, entry_p, entry_dims, COLOR_RED);
                } break ;
                case MAZE_ENTRY_ROOM_2: {
                    struct v2r32 entry_p = v2r32(c * entry_dims.x + entry_offset.x, r * entry_dims.y + entry_offset.y);
                    window__draw_rectangle(window, entry_p, entry_dims, COLOR_GREEN);
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
            console__log(console, "%c", maze__get_entry(self, v2u16(c, r)));
        }
        console__log(console, "\n");
    }
}

// TODO(david): asset loader
static void assets__load(struct bitmap* bitmap_generate, struct console* console) {
    struct file_reader reader;

    struct bmp_loader bmp_loader;

    const char* filename = "assets/art.bmp";
    file_reader__create(&reader, filename);

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

    struct v2u16 maze_dims = v2u16(12, 3);
    if (maze__create(&maze, &maze_dims, 42) == false) {
        console__fatal(&console, "failed to create the maze");
    }

    maze__build(&maze);
    // maze__build2(&maze);

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

    struct v2r32 button_dims_within_main_canvas_overlay   = v2r32(50.0f, 50.0f);
    struct v2r32 button_offset_within_main_canvas_overlay = v2r32(button_dims_within_main_canvas_overlay.x / 5.0f, button_dims_within_main_canvas_overlay.y / 5.0f);

    u32 button_within_main_canvas_overlay_counter = 0;

    struct button button_increment_maze_dimensions;
    struct v2r32 button_increment_maze_dimensions_p = v2r32(
        button_within_main_canvas_overlay_counter * (button_dims_within_main_canvas_overlay.x + button_offset_within_main_canvas_overlay.x),
        0.0f
    );
    struct v2r32 button_increment_maze_dimensions_dims = button_dims_within_main_canvas_overlay;
    button__create(&button_increment_maze_dimensions, button_increment_maze_dimensions_p, button_increment_maze_dimensions_dims, COLOR_BLUE, COLOR_YELLOW, COLOR_RED);
    ui_group__push_button(&ui_group_overlay, &button_increment_maze_dimensions);

    ++button_within_main_canvas_overlay_counter;

    struct button button_decrement_maze_dimensions;
    struct v2r32 button_decrement_maze_dimensions_p = v2r32(
        button_within_main_canvas_overlay_counter * (button_dims_within_main_canvas_overlay.x + button_offset_within_main_canvas_overlay.x),
        0.0f
    );
    struct v2r32 button_decrement_maze_dimensions_dims = button_dims_within_main_canvas_overlay;
    button__create(&button_decrement_maze_dimensions, button_decrement_maze_dimensions_p, button_decrement_maze_dimensions_dims , COLOR_BLUE, COLOR_YELLOW, COLOR_RED);
    ui_group__push_button(&ui_group_overlay, &button_decrement_maze_dimensions);

    while (window__does_exist(&window)) {
        // u64 time_start = __rdtsc();
        window__poll_inputs(&window);

        if (window__key_is_pressed(&window, KEY_ESCAPE)) {
            window__close_next_frame(&window);
        }

        if (window__key_is_pressed(&window, KEY_RIGHT)) {
            maze__build_advance(&maze);
        }

        ui_group__update_and_render(&ui_group_overlay);
        ui_group__update_and_render(&ui_group_main_canvas);
        maze__render(&maze, &window, &ui_group_main_canvas, &maze_holder_box);

        if (button__is_pressed(&button_increment_maze_dimensions)) {
            maze__destroy(&maze);
            ++maze_dims.x;
            ++maze_dims.y;
            if (maze__create(&maze, &maze_dims, rand()) == false) {
                console__fatal(&console, "failed to create the maze");
            }
            maze__build(&maze);
        }
        if (button__is_pressed(&button_decrement_maze_dimensions)) {
            maze__destroy(&maze);
            --maze_dims.x;
            --maze_dims.y;
            if (maze__create(&maze, &maze_dims, rand()) == false) {
                console__fatal(&console, "failed to create the maze");
            }
            maze__build(&maze);
        }

        maze__build_advance(&maze);

        window__end_draw(&window);
        // u64 time_end = __rdtsc();
        // console__log(&console, "Time taken for frame in cy: %lu\n", time_end - time_start);
    }

    ui_group__destroy(&ui_group_overlay);
    window__destroy(&window);
    window__deinit_module(&window);
    console__deinit_module(&console);
}
