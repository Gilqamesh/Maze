#include "world_position.h"

#include "world_grid.h"

struct v2r32 world_position__to_relative_p(struct world_position p, struct world_position relative_p) {
    struct v2r32 world_grid_dims = world_grid__dims();

    struct v2r32 result = v2r32(
        (p.global_p.x - relative_p.global_p.x) * world_grid_dims.x + (p.local_p.x - relative_p.local_p.x),
        (p.global_p.y - relative_p.global_p.y) * world_grid_dims.y + (p.local_p.y - relative_p.local_p.y)
    );

    return result;
}

struct world_position world_position__from_relative_p(struct v2r32 p, struct world_position relative_p) {
    struct v2r32 world_grid_dims = world_grid__dims();

    struct v2i32 k = v2i32(
        (i32) floor((p.x + relative_p.local_p.x) / world_grid_dims.x + 1 / 2.0f),
        (i32) floor((p.y + relative_p.local_p.y) / world_grid_dims.y + 1 / 2.0f)
    );

    struct world_position result;

    result.local_p = v2r32(
        p.x + relative_p.local_p.x - (r32) k.x * world_grid_dims.x,
        p.y + relative_p.local_p.y - (r32) k.y * world_grid_dims.y
    );

    result.global_p = v2i32(
        k.x + relative_p.global_p.x,
        k.y + relative_p.global_p.y
    );

    return result;
}

struct world_position world_position__add_relative_p(struct world_position p, struct v2r32 relative_p) {
    return world_position__from_relative_p(relative_p, p);
}
