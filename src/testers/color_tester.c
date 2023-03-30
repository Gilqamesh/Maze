#include "../window/window.h"
#include "../math/lerp.h"
#include "../math/clamp.h"
#include "../math/r32.h"

int WinMain(HINSTANCE app_handle, HINSTANCE prev_instance, LPSTR cmd_line, int show_cmd) {
    struct console console;

    if (!console__init_module(&console, 512)) {
        // ExitProcess(EXIT_FAILURE);
    }

    struct window window;
    window__init_module(&window, &console);

    window__create(&window, app_handle, "Color Tester", v2u32(100, 100), v2u32(800, 600));

    struct v2r32 rec_p = v2r32(
        (r32) window.dims.x / 2.0f,
        (r32) window.dims.y / 2.0f
    );
    struct v2r32 rec_dims = v2r32(
        240.5f, 160.3f
    );
    struct v2r32 rec_center_p = v2r32(
        rec_p.x + rec_dims.x / 2.0f,
        rec_p.y + rec_dims.y / 2.0f
    );
    enum color rec_color_original = COLOR_RED;
    r32 rec_color_overlap = 1.0f;

    r32 r = 0.0f;

    while (window__does_exist(&window)) {
        window__clear_screen(&window, COLOR_WHITE);
        window__poll_inputs(&window);

        struct v2i32 mouse_p = window__mouse_get_position(&window);
        struct v2r32 mouse_p_v2r32 = v2r32((r32) mouse_p.x, (r32) mouse_p.y);
        struct v2r32 delta_p = v2r32__sub_v2r32(mouse_p_v2r32, rec_center_p);
        delta_p = v2r32__normalize(delta_p);
        delta_p = v2r32__scale_v2r32(delta_p, v2r32(0.04f, 0.04f));

        if (window__key_is_down(&window, MOUSE_LBUTTON)) {
            rec_center_p = v2r32__add_v2r32(rec_center_p, delta_p);
            rec_p = v2r32__add_v2r32(rec_p, delta_p);
        }

        r32 rec_color_overlap_d = 0.0005f;
        if (window__key_is_down(&window, KEY_LEFT)) {
            r += 0.1f;
            rec_color_overlap -= rec_color_overlap_d;
        }
        if (window__key_is_down(&window, KEY_RIGHT)) {
            r -= 0.1f;
            rec_color_overlap += rec_color_overlap_d;
        }
        rec_color_overlap = clamp__r32(0.0f, rec_color_overlap, 1.0f);

        u32 rec_color = lerp__color(COLOR_WHITE, rec_color_overlap, rec_color_original);

        window__draw_rectangle(&window, rec_p, rec_dims, rec_color);

        window__end_draw(&window);
    }

    window__deinit_module(&window);

    console__deinit_module(&console);
}
