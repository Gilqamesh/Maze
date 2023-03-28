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

    struct world_position view_port_center_p;
    struct v2r32 view_port_half_dims;

    struct v2u32 window_client_top_left_p;
    struct v2u32 window_client_dims;
};

DLLEXPORT void camera__create(
    struct camera* self,
    struct window* window,
    struct world* world,
    struct world_position view_port_center_p,
    struct v2r32 view_port_half_dims,
    struct v2u32 window_client_top_left_p,
    struct v2u32 window_client_dims
);

DLLEXPORT void camera__destroy(struct camera* self);

DLLEXPORT void camera__update_view_port_p_absolute(struct camera* self, struct world_position new_view_port_center_p);
// @brief relative to itself
DLLEXPORT void camera__update_view_port_p_relative(struct camera* self, struct v2r32 view_port_center_dp);

DLLEXPORT void camera__update_view_port_half_dims_absolute(struct camera* self, struct v2r32 new_view_port_half_dims);
// @brief relative to itself
DLLEXPORT void camera__update_view_port_half_dims_relative(struct camera* self, struct v2r32 d_view_port_half_dims);

DLLEXPORT void camera__render(struct camera* self);
