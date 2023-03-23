#pragma once

#include "world_defs.h"

#include "entity.h"
#include "../math/v2i32.h"

struct world_grid_index {
    struct v2i32 p;
};

/*
    Regarding world_grid's implicit coordinate system:
    Center point is (0, 0).
*/
struct world_grid {
    struct entity** entities;
    u32 entities_size;
    u32 entities_fill;

    struct world_grid* next; // in case there is a collision in the hash-table that stores it

    struct world_grid_index index;
};

void world_grid__create(struct world_grid* self, struct world_grid_index index);
