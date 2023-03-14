#pragma once

#include "ui_defs.h"
#include "../math/v2u32.h"
#include "../window/window.h"

struct box {
    struct box*    next;
    struct v2r32   p;
    struct v2r32   dims;
    enum color     color;
};

DLLEXPORT void box__create(struct box* self, struct v2r32 p, struct v2r32 dims, enum color color);
DLLEXPORT void box__destroy(struct box* self);

DLLEXPORT enum color box__get_color(struct box* self);

DLLEXPORT void box__update_and_render(struct box* self, struct window* window, struct v2u32 mp, struct v2r32 ui_group_offset, struct v2r32 ui_group_dims, struct v2r32 ui_group_scale);
