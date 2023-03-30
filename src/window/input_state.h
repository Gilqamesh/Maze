#pragma once

#include "window_defs.h"

#include "../math/v2i32.h"
#include "key.h"

struct button_state {
    u32  half_transition_count;
    bool ended_down;
};

struct mouse_state {
    struct v2i32        mouse_p;
    struct v2i32        mouse_p_prev;
    i32                 mouse_wheel_delta;
};

struct input_state {
    struct button_state buttons[_KEY_SIZE];
    struct mouse_state  mouse;
};
