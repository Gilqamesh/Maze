#include "window_utils.h"
#include "Windowsx.h"

static inline void _window_button_state_process(struct window* self, bool is_down, enum key key) {
    struct button_state* button_state = &self->input_state.buttons[key];
    if (button_state->ended_down != is_down)
    {
        ++button_state->half_transition_count;
        button_state->ended_down = is_down;
    }
}

static inline struct window* _window_get_from_handle(HWND window_handle) {
    struct window* window = (struct window*) GetWindowLongPtrA(window_handle, GWLP_USERDATA);
    if (window == NULL) {
        ExitProcess(APP_ERROR_WINDOW_CALLBACK);
    }

    return window;
}

LRESULT CALLBACK _window_callback(HWND window_handle, UINT MessageCode, WPARAM WParam, LPARAM LParam) {
    LRESULT result = 0;

    switch (MessageCode) {
        case WM_CREATE: {
            struct window* window = _window_get_from_handle(window_handle);
            CREATESTRUCT *create_window_params = (CREATESTRUCT *)LParam;
            if (create_window_params == NULL) {
                console__fatal(window->console, "in '_window_callback': create_window_params is NULL\n");
            }

            SetWindowLongPtrW(window_handle, GWLP_USERDATA, (LONG_PTR)create_window_params->lpCreateParams);
        } break ;
        case WM_CLOSE: {
            struct window* window = _window_get_from_handle(window_handle);
            window->destroy_next_frame = true;
        } break ;
        case WM_ACTIVATEAPP: {
        } break ;
        case WM_DESTROY: {
            PostQuitMessage(0);
        } break ;
        case WM_MOUSEMOVE: {
            struct window* window = _window_get_from_handle(window_handle);
            window->mouse_p = v2u32(GET_X_LPARAM(LParam), GET_Y_LPARAM(LParam));
        } break ;
        case WM_LBUTTONDOWN: {
            SetCapture(window_handle);
            struct window* window = _window_get_from_handle(window_handle);
            _window_button_state_process(window, true, MOUSE_LBUTTON);
        } break ;
        case WM_LBUTTONUP: {
            struct window* window = _window_get_from_handle(window_handle);
            if (ReleaseCapture() == FALSE) {
                console__fatal(window->console, "in '_window_callback': SetCapture returned NULL, GetLastError() = %d\n", GetLastError());
            }
            _window_button_state_process(window, false, MOUSE_RBUTTON);
        } break ;
        // case WM_LBUTTONDBLCLK: {
        // } break ;
        case WM_KEYUP:
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP: {
            // INVALID_CODE_PATH;
        } break ;
        default: {
            result = DefWindowProcW(window_handle, MessageCode, WParam, LParam);
        };
    }

    return result;
}

void _window_input_dispatch(struct window* self, LPARAM l_param, WPARAM w_param) {
    bool was_down = ((l_param & (1 << 30)) != 0);
    bool is_down  = ((l_param & (1 << 31)) == 0);
    u32 virtual_key_code = w_param;
    switch (virtual_key_code)
    {
        case VK_BACK: {
            _window_button_state_process(self, is_down, KEY_BACKSPACE);
        } break ;
        case VK_TAB: {
            _window_button_state_process(self, is_down, KEY_TAB);
        } break ;
        case VK_RETURN: {
            _window_button_state_process(self, is_down, KEY_ENTER);
        } break ;
        case VK_SHIFT: {
            _window_button_state_process(self, is_down, KEY_SHIFT);
        } break ;
        case VK_CONTROL: {
            _window_button_state_process(self, is_down, KEY_CTRL);
        } break ;
        case VK_MENU: {
            _window_button_state_process(self, is_down, KEY_ALT);
        } break ;
        case VK_ESCAPE: {
            _window_button_state_process(self, is_down, KEY_ESCAPE);
        } break ;
        case VK_SPACE: {
            _window_button_state_process(self, is_down, KEY_SPACE);
        } break ;
        case VK_LEFT: {
            _window_button_state_process(self, is_down, KEY_LEFT);
        } break ;
        case VK_UP: {
            _window_button_state_process(self, is_down, KEY_UP);
        } break ;
        case VK_RIGHT: {
            _window_button_state_process(self, is_down, KEY_RIGHT);
        } break ;
        case VK_DOWN: {
            _window_button_state_process(self, is_down, KEY_DOWN);
        } break ;
        case VK_LSHIFT: {
            _window_button_state_process(self, is_down, KEY_LSHIFT);
        } break ;
        case VK_RSHIFT: {
            _window_button_state_process(self, is_down, KEY_RSHIFT);
        } break ;
        case VK_LCONTROL: {
            _window_button_state_process(self, is_down, KEY_LCTRL);
        } break ;
        case VK_RCONTROL: {
            _window_button_state_process(self, is_down, KEY_RCTRL);
        } break ;
        case VK_LMENU: {
            _window_button_state_process(self, is_down, KEY_LALT);
        } break ;
        case VK_RMENU: {
            _window_button_state_process(self, is_down, KEY_RALT);
        } break ;
        default: {
            console__log(self->console, "Key is not supported, virtual key code %lu\n", virtual_key_code);
        } break ;
    }
}
