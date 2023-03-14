#pragma once

#include "ui_defs.h"
#include "../math/v2u32.h"
#include "../math/v2r32.h"
#include "../window/window.h"
#include "button.h"

struct ui_group {
    struct window* window;
    struct button* buttons;
    struct v2r32   p;
    struct v2r32   dims;
    struct v2u32   original_window_dims;
    enum color     color_passive;
    enum color     color_hovered;
    bool           is_hovered;
};

DLLEXPORT void ui_group__create(struct ui_group* self, struct window* window, struct v2r32 p, struct v2r32 dims, enum color color_passive, enum color color_hovered);
DLLEXPORT void ui_group__destroy(struct ui_group* self);

DLLEXPORT void ui_group__push_button(struct ui_group* self, struct button* button);

DLLEXPORT void ui_group__update_and_render(struct ui_group* self);

DLLEXPORT enum color ui_group__get_color(struct ui_group* self);
