#pragma once

#include "world_defs.h"

#include "world.h"
#include "world_position.h"
#include "renderer.h"

#include "../window/window.h"

struct camera {
    struct window* window;
    struct world* world;
    struct renderer renderer;

    struct world_position viewport_center_p;
    struct v2r32 viewport_half_dims;

    struct v2r32 window_client_top_left_p_normalized;
    struct v2r32 window_client_bot_right_p_normalized;
};

DLLEXPORT void camera__create(
    struct camera* self,
    struct window* window,
    struct world* world,
    struct world_position viewport_center_p,
    struct v2r32 viewport_half_dims,
    struct v2r32 window_client_top_left_p_normalized,
    struct v2r32 window_client_bot_right_p_normalized
);

DLLEXPORT void camera__destroy(struct camera* self);

DLLEXPORT void camera__update_viewport_p_absolute(struct camera* self, struct world_position new_viewport_center_p);
// @brief relative to itself
DLLEXPORT void camera__update_viewport_p_relative(struct camera* self, struct v2r32 viewport_center_dp);

DLLEXPORT void camera__update_viewport_half_dims_absolute(struct camera* self, struct v2r32 new_viewport_half_dims);
// @brief relative to itself
DLLEXPORT void camera__update_viewport_half_dims_relative(struct camera* self, struct v2r32 d_viewport_half_dims);

DLLEXPORT bool camera__is_p_in_window_client_area(struct camera* self, struct v2r32 p);

DLLEXPORT void camera__render(struct camera* self);
