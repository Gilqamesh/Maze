#pragma once

#include "window_defs.h"
#include "window.h"

void _window_input_dispatch(struct window* self, LPARAM l_param, WPARAM w_param);

LRESULT CALLBACK _window_callback(HWND window_handle, UINT MessageCode, WPARAM WParam, LPARAM LParam);
