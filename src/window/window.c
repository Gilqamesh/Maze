#include "window.h"
#include "window_utils.h"
#include "../math/clamp.h"
#include "../math/abs_diff.h"
#include "../readers/bitmap.h"

bool window__init_module(struct window* self, struct console* console) {
    self->console = console;

    return true;
}

void window__deinit_module(struct window* self) {
    window__destroy(self);
}

bool window__create(struct window* self, HINSTANCE app_handle, const char* window_name, struct v2u32 window_pos, struct v2u32 window_dims) {
    self->destroy_next_frame = false;
    self->dims               = window_dims;
    memset(&self->input_state, 0, sizeof(self->input_state));

    if (bit_buffer__create(&self->frame_buffer, window_dims) == false) {
        return false;
    }

    WNDCLASSA window_class = { 0 };
    window_class.style = 0;
    window_class.lpfnWndProc = &_window_callback;
    window_class.hInstance = app_handle;

    const char* class_name = "window_class_name";
    if (window_class.hInstance == NULL) {
        console__fatal(self->console, "in 'window__create': GetModuleHandleW failed, GetLastError() = %d\n", GetLastError());
        return false;
    }
    window_class.lpszClassName = class_name;

    if (RegisterClassA(&window_class) == 0) {
        // TODO(david): probably already registered
        console__fatal(self->console, "in 'window__create': RegisterClassW failed, GetLastError() = %d\n", GetLastError());
        return false;
    }

    const DWORD window_style = WS_OVERLAPPEDWINDOW;
    const DWORD extended_window_style = 0;
    const HWND parent_window_handle = NULL;
    const HMENU menu_handle = NULL;

    self->window_handle = CreateWindowExA(
        extended_window_style, class_name, window_name,
        window_style, window_pos.x, window_pos.y,
        window_dims.x, window_dims.y, parent_window_handle,
        menu_handle, app_handle, self
    );
    
    if (self->window_handle == NULL) {
        console__fatal(self->console, "in 'window__create': CreateWindowExW failed, GetLastError() = %d\n", GetLastError());
        return false;
    }

    window__show(self);

    return true;
}

void window__destroy(struct window* self) {
    bit_buffer__destroy(&self->frame_buffer);

    self->window_handle = NULL;
}

void window__close_next_frame(struct window* self) {
    self->destroy_next_frame = true;
}

bool window__does_exist(struct window* self) {
    if (self->destroy_next_frame) {
        self->destroy_next_frame = false;
        DestroyWindow(self->window_handle);
        MSG Message = { 0 };
        while (PeekMessageA(&Message, self->window_handle, 0, 0, PM_REMOVE)) {
            TranslateMessage(&Message);
            DispatchMessageA(&Message);
        }

        return false;
    } else {
        return self->window_handle != NULL;
    }
}

void window__show(struct window* self) {
    ShowWindow(self->window_handle, SW_SHOW);
}

void window__poll_inputs(struct window* self) {
    self->input_state.mouse.mouse_p_prev = self->input_state.mouse.mouse_p;
    self->input_state.mouse.mouse_wheel_delta = 0;

    for (u32 button_index = 0; button_index < ARRAY_SIZE(self->input_state.buttons); ++button_index) {
        self->input_state.buttons[button_index].half_transition_count = 0;
    }

    MSG message = { 0 };
    while (PeekMessageA(&message, self->window_handle, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        switch (message.message) {
            case WM_KEYUP:
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP: {
                _window_input_dispatch(self, message.lParam, message.wParam);
            } break ;
            default: {
                DispatchMessageA(&message);
            }
        }
    }
}

void window__end_draw(struct window* self) {
    HDC window_device_context_handle = GetDC(self->window_handle);
    if (window_device_context_handle == NULL) {
        console__fatal(self->console, "in 'window__end_draw': GetDC failed\n");
    }

    bit_buffer__blit_to_window(&self->frame_buffer, self, window_device_context_handle);

    ReleaseDC(self->window_handle, window_device_context_handle);

    InvalidateRect(self->window_handle, NULL, FALSE);
}

struct v2u32 window__mouse_get_position(struct window* self) {
    return self->input_state.mouse.mouse_p;
}

struct v2i32 window__mouse_get_delta(struct window* self) {
    return v2i32(
        (i32) self->input_state.mouse.mouse_p.x - (i32) self->input_state.mouse.mouse_p_prev.x,
        (i32) self->input_state.mouse.mouse_p.y - (i32) self->input_state.mouse.mouse_p_prev.y
    );
}

i32 window__mouse_get_wheel_delta(struct window* self) {
    return self->input_state.mouse.mouse_wheel_delta;
}

u32 window__key_is_pressed(struct window* self, enum key key) {
    u32 result = 0;

    const u32 half_transition_count = self->input_state.buttons[key].half_transition_count;

    if (window__key_is_down(self, key)) {
        result = ((half_transition_count + 1) >> 1);
    } else {
        result = (half_transition_count >> 1);
    }

    return result;
}

bool window__key_is_down(struct window* self, enum key key) {
    return self->input_state.buttons[key].ended_down;
}

void window__clear_screen(struct window* self, enum color color) {
    window__draw_rectangle(
        self,
        v2r32(0.0f, 0.0f),
        v2r32(self->frame_buffer.dims.x, self->frame_buffer.dims.y),
        color
    );
}

void window__draw_pixel(struct window* self, struct v2u32 position, enum color color) {
    position = clamp__v2u32(v2u32(0, 0), position, self->frame_buffer.dims);

    *((u32*) (self->frame_buffer.buffer) + position.y * self->frame_buffer.dims.x + position.x) = color;
}

void window__draw_rectangle(struct window* self, struct v2r32 top_left_p, struct v2r32 dims, enum color color) {
    // todo: move this to renderer probably and do anti-aliasing there
    struct v2u32 start_p = v2u32(
        clamp__u32(
            0,
            (u32) round(clamp__r32(0.0f, top_left_p.x, (r32) self->frame_buffer.dims.x)),
            self->frame_buffer.dims.x - 1
        ),
        clamp__u32(
            0,
            (u32) round(clamp__r32(0.0f, top_left_p.y, (r32) self->frame_buffer.dims.y)),
            self->frame_buffer.dims.y - 1
        )
    );

    struct v2u32 end_p = v2u32(
        clamp__u32(
            0,
            (u32) round(clamp__r32(0.0f, top_left_p.x + dims.x, (r32) self->frame_buffer.dims.x)),
            self->frame_buffer.dims.x - 1
        ),
        clamp__u32(
            0,
            (u32) round(clamp__r32(0.0f, top_left_p.y + dims.y, (r32) self->frame_buffer.dims.y)),
            self->frame_buffer.dims.y - 1
        )
    );

    struct v2u32 clamped_dims = abs_diff__v2u32(start_p, end_p);

    u32  bit_buffer_stride = self->frame_buffer.dims.x;
    u32* bit_buffer_p = ((u32*) self->frame_buffer.buffer) + start_p.y * bit_buffer_stride + start_p.x;

    for (u32 row_offset = 0; row_offset <= clamped_dims.y; ++row_offset) {
        for (u32 col_offset = 0; col_offset <= clamped_dims.x; ++col_offset) {
            *(bit_buffer_p + col_offset) = *(u32 *)&color;
        }
        bit_buffer_p += bit_buffer_stride;
    }
}

void window__draw_bitmap(struct window* self, struct v2u32 top_left_p, struct bitmap* bitmap) {
    struct v2u32 start_p = clamp__v2u32(v2u32(0, 0), top_left_p, v2u32(self->frame_buffer.dims.x - 1, self->frame_buffer.dims.y - 1));
    struct v2u32 end_p   = clamp__v2u32(v2u32(0, 0), v2u32(start_p.x + bitmap->dims.x, start_p.y + bitmap->dims.y), v2u32(self->frame_buffer.dims.x - 1, self->frame_buffer.dims.y - 1));

    struct v2u32 clamped_dims = abs_diff__v2u32(start_p, end_p);

    u32  bit_buffer_stride = self->frame_buffer.dims.x;
    u32* bit_buffer_p = (u32*) self->frame_buffer.buffer + start_p.y * bit_buffer_stride + start_p.x;

    u32  bitmap_stride = bitmap->dims.x;
    u32* bitmap_p = (u32*) bitmap->data;

    for (u32 row_offset = 0; row_offset < clamped_dims.y; ++row_offset) {
        for (u32 col_offset = 0; col_offset < clamped_dims.x; ++col_offset) {
            *(bit_buffer_p + col_offset) = *(bitmap_p + col_offset);
        }
        bit_buffer_p += bit_buffer_stride;
        bitmap_p     += bitmap_stride;
    }
}
