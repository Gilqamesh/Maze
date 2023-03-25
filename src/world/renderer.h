#pragma once

#include "world_defs.h"

#include "world_position.h"

#include "../math/v2r32.h"
#include "../window/window.h"
#include "../window/color.h"

struct renderer {
    struct window* window;
    struct world_position center_p;
    struct v2r32 half_dims;
};

DLLEXPORT void renderer__create(struct renderer* self, struct window* window, struct world_position center_p, struct v2r32 half_dims);
DLLEXPORT void renrerer__destroy(struct renderer* self);

DLLEXPORT void renderer__push_pixel(struct renderer* self, struct v2r32 pixel, enum color color);
DLLEXPORT void renderer__push_rectangle(struct renderer* self, struct v2r32 position, struct v2r32 dims, enum color color);
