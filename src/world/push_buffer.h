#pragma once

#include "world_defs.h"

#include "../math/v3r32.h"
#include "../window/color.h"

struct push_buffer_rectangle {
    struct v3r32 center_p;
    struct v3r32 half_dims;
    enum color color;
};

DLLEXPORT struct push_buffer_rectangle push_buffer_rectangle__create(
    struct v3r32 center_p,
    struct v3r32 half_dims,
    enum color color
);

struct push_buffer {
    struct push_buffer_rectangle* rectangles;
    u32 rectangles_size;
    u32 rectangles_fill;
};

DLLEXPORT void push_buffer__create(struct push_buffer* self);
DLLEXPORT void push_buffer__destroy(struct push_buffer* self);

DLLEXPORT void push_buffer__pop(struct push_buffer* self);
DLLEXPORT void push_buffer__clear(struct push_buffer* self);

DLLEXPORT void push_buffer__push_rectangle(
    struct push_buffer* self,
    struct v3r32 center_p,
    struct v3r32 half_dims,
    enum color color
);

DLLEXPORT void push_buffer__sort_rectangles(
    struct push_buffer* self
);
