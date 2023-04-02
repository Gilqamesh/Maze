#pragma once

#include "ui_defs.h"
#include "../math/v2u32.h"
#include "../window/window.h"

// todo(david): make this hold a texture and/or text, sound etc.
struct button {
    struct button* next;
    struct v2r32   p;
    struct v2r32   dims;
    enum color     color_passive;
    enum color     color_hovered;
    enum color     color_active;
    bool           is_pressed;
    bool           is_hovered;
};

DLLEXPORT void button__create(
    struct button* self,
    struct v2r32 p,
    struct v2r32 dims,
    enum color color_passive,
    enum color color_hovered,
    enum color color_active
);
DLLEXPORT void button__destroy(struct button* self);

DLLEXPORT enum color button__get_color(struct button* self);
DLLEXPORT bool button__is_pressed(struct button* self);

DLLEXPORT void button__update_and_render(
    struct button* self,
    struct window* window,
    struct v2i32 mp,
    struct v2r32 ui_group_offset,
    struct v2r32 ui_group_dims,
    struct v2r32 ui_group_scale,
    bool is_mouse_lbutton_pressed
);
