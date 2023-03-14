#pragma once

#include "ui_defs.h"
#include "../math/v2u32.h"
#include "../window/window.h"

// todo(david): make this hold a texture and/or text, sound etc.
struct button {
    struct window* window;
    struct v2r32   p;
    struct v2r32   dims;
    struct v2u32   original_window_dims;
    enum color     color_passive;
    enum color     color_hovered;
    enum color     color_active;
    bool           is_pressed;
    bool           is_hovered;
};

// @param p
// @param dims
DLLEXPORT void button__create(struct button* self, struct window* window, struct v2r32 p, struct v2r32 dims, enum color color_passive, enum color color_hovered, enum color color_active);
DLLEXPORT void button__destroy(struct button* self);

DLLEXPORT void button__update(struct button* self);

DLLEXPORT enum color button__get_color(struct button* self);
DLLEXPORT bool button__is_pressed(struct button* self);

DLLEXPORT void button__render(struct button* self);