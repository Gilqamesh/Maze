#pragma once

#include "world_defs.h"

#include "world_position.h"

#include "../math/v2r32.h"
#include "../window/window.h"
#include "../window/color.h"

#include "world.h"
#include "push_buffer.h"

struct renderer {
    struct push_buffer push_buffer;
};

DLLEXPORT void renderer__create(struct renderer* self);
DLLEXPORT void renderer__destroy(struct renderer* self);

DLLEXPORT void renderer__push_rectangle(struct renderer* self, struct v2r32 center_p, struct v2r32 half_dims, enum color color);

DLLEXPORT void renderer__clear_push_buffer(struct renderer* self);

// todo: remove this, and move entity pushing to camera
DLLEXPORT void renderer__push_entities(
    struct renderer* self,
    struct window* window,
    struct world* world,
    struct world_position center_p,
    struct v2r32 half_dims
);

DLLEXPORT void renderer__render(
    struct renderer* self,
    struct window* window,
    struct world* world,
    struct world_position center_p,
    struct v2r32 half_dims
);
