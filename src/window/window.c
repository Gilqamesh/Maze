#include "window.h"
#include "window_utils.h"

bool window__init_module(struct window* self, struct console* console) {
    self->console = console;

    return true;
}

void window__deinit_module(struct window* self) {
    window__destroy(self);
}

bool window__create(struct window* self, HINSTANCE app_handle, const char* window_name, struct v2u32 window_pos, struct v2u32 window_dims) {
    self->destroy_next_frame = false;

    bit_buffer__create(&self->frame_buffer, window_dims);

    const DWORD window_style = WS_OVERLAPPEDWINDOW;

    WNDCLASSA window_class = { 0 };
    window_class.style = CS_DBLCLKS;
    window_class.lpfnWndProc = &_window_callback;
    window_class.hInstance = app_handle;

    const char* class_name = "window_class_name";
    if (window_class.hInstance == NULL) {
        console__fatal(self->console, "in 'window__create': GetModuleHandleW failed, GetLastError() = %d\n", GetLastError());
    }
    window_class.lpszClassName = class_name;

    if (RegisterClassA(&window_class) == 0) {
        // TODO(david): probably already registered
        console__fatal(self->console, "in 'window__create': RegisterClassW failed, GetLastError() = %d\n", GetLastError());
    }

    self->window_handle = CreateWindowA(
        class_name, window_name,
        window_style, window_pos.x, window_pos.y,
        window_dims.x, window_dims.y, NULL, NULL,
        app_handle, self
    );
    
    if (self->window_handle == NULL) {
        console__fatal(self->console, "in 'window__create': CreateWindowExW failed, GetLastError() = %d\n", GetLastError());
    }

    ShowWindow(self->window_handle, SW_SHOW);

    return true;
}

void window__destroy(struct window* self) {
    if (PostMessageW(self->window_handle, WM_CLOSE, 0, 0) == 0) {
        console__fatal(self->console, "in 'window__destroy': PostMessageW returned 0, GetLastError() = %d\n", GetLastError());
    }

    bit_buffer__destroy(&self->frame_buffer);
}

bool window__does_exist(struct window* self) {
    if (self->destroy_next_frame) {
        self->destroy_next_frame = false;
        DestroyWindow(self->window_handle);
        MSG Message = { 0 };
        while (PeekMessageW(&Message, self->window_handle, 0, 0, PM_REMOVE)) {
            TranslateMessage(&Message);
            DispatchMessageW(&Message);
        }

        bit_buffer__destroy(&self->frame_buffer);

        return false;
    } else {
        return self->window_handle != NULL;
    }
}

void window__show(struct window* self) {
}

void window__poll_inputs(struct window* self) {
    self->mouse_p_prev = self->mouse_p;

    for (u32 button_index = 0; button_index < ARRAY_SIZE(self->input_state.buttons); ++button_index) {
        self->input_state.buttons[button_index].half_transition_count = 0;
    }

    MSG message = { 0 };
    while (PeekMessageW(&message, self->window_handle, 0, 0, PM_REMOVE)) {
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
    HDC window_device_context = GetDC(self->window_handle);
    if (window_device_context == NULL) {
        console__fatal(self->console, "in 'window__end_draw': GetDC failed\n");
    }

    RECT client_rect;
    if (GetClientRect(self->window_handle, &client_rect) == 0) {
        console__fatal(self->console, "in 'window__end_draw': GetClientRect failed, GetLastError = %d\n", GetLastError());
    }

    bit_buffer__blit_to_window(&self->frame_buffer, window_device_context, self->dims);

    ReleaseDC(self->window_handle, window_device_context);

    InvalidateRect(self->window_handle, NULL, FALSE);
}

struct v2u32 window__mouse_get_position(struct window* self) {
    return self->mouse_p;
}

u32 window__is_key_pressed(struct window* self, enum key key) {
    u32 result = 0;

    const u32 half_transition_count = self->input_state.buttons[key].half_transition_count;

    if (window__is_key_down(self, key)) {
        result = ((half_transition_count + 1) >> 1);
    } else {
        result = (half_transition_count >> 1);
    }

    return result;
}

bool window__is_key_down(struct window* self, enum key key) {
    return self->input_state.buttons[key].ended_down;
}

void window__draw_rectangle(struct window* self, struct v2u32 position, struct v2u32 dims, enum color color) {
    struct bit_buffer *bit_buffer = &self->frame_buffer;

    for (u32 y = position.y; y < position.y + dims.y && y < bit_buffer->dims.y; ++y) {
        for (u32 x = position.x; x < position.x + dims.x && x < bit_buffer->dims.x; ++x) {
            ((u32 *)bit_buffer->buffer)[y * bit_buffer->dims.x + x] = *(u32 *)&color;
        }
    }
}
