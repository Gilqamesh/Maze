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

DLLEXPORT void renderer__push_rectangle_centered(struct renderer* self, struct v2r32 center_p, struct v2r32 half_dims, enum color color);
DLLEXPORT void renderer__push_rectangle_top_left(struct renderer* self, struct v2r32 top_left_p, struct v2r32 dims, enum color color);

DLLEXPORT void renderer__clear_push_buffer(struct renderer* self);

DLLEXPORT void renderer__render(
    struct renderer* self,
    struct window* window,
    struct world* world,
    struct world_position viewport_p,
    struct v2r32 viewport_half_dims,
    struct v2r32 window_client_top_left_p_normalized,
    struct v2r32 window_client_bot_right_p_normalized
);
