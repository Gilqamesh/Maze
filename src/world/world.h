#pragma once

#include "world_defs.h"

#include "world_grid.h"

struct world {
    struct world_grid** world_grids;
    u32 world_grids_size;
};

DLLEXPORT void world__create(struct world* self);

DLLEXPORT void world__destroy(struct world* self);

DLLEXPORT struct world_grid* world__get_grid(struct world* self, struct v2i32 index);
