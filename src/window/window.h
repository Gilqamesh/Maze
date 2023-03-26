#ifndef MODULE_WINDOW_HPP
# define MODULE_WINDOW_HPP

#include "../console/console.h"
#include "../math/v2r32.h"
#include "../math/v2i32.h"
#include "../math/v2u32.h"
#include "color.h"
#include "input_state.h"
#include "bit_buffer.h"

struct window {
    struct console*        console;
    HWND                   window_handle;
    struct v2u32           dims;
    struct input_state     input_state;
    bool                   destroy_next_frame;
    struct bit_buffer      frame_buffer;
};

DLLEXPORT bool window__init_module(struct window* self, struct console* console);
DLLEXPORT void window__deinit_module(struct window* self);

// @brief creates a window
DLLEXPORT bool window__create(struct window* self, HINSTANCE app_handle, const char* window_name, struct v2u32 window_pos, struct v2u32 window_dims);
DLLEXPORT void window__destroy(struct window* self);

DLLEXPORT void window__close_next_frame(struct window* self);
DLLEXPORT bool window__does_exist(struct window* self);
DLLEXPORT void window__show(struct window* self);
DLLEXPORT void window__poll_inputs(struct window* self);

DLLEXPORT void window__end_draw(struct window* self);

DLLEXPORT struct v2u32 window__mouse_get_position(struct window* self);
DLLEXPORT struct v2i32 window__mouse_get_delta(struct window* self);
DLLEXPORT i32 window__mouse_get_wheel_delta(struct window* self);
DLLEXPORT u32 window__key_is_pressed(struct window* self, enum key key);
DLLEXPORT bool window__key_is_down(struct window* self, enum key key);


DLLEXPORT void window__clear_screen(struct window* self, enum color color);
DLLEXPORT void window__draw_pixel(struct window* self, struct v2u32 position, enum color color);
DLLEXPORT void window__draw_rectangle(struct window* self, struct v2r32 top_left_p, struct v2r32 dims, enum color color);
DLLEXPORT void window__draw_bitmap(struct window* self, struct v2u32 top_left_p, struct bitmap* bitmap);

#endif
