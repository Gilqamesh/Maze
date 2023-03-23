#pragma once

#include "world_defs.h"

#include "world_position.h"
#include "../math/v2u32.h"

/*
    Regarding entity position:
    Has a center point that is relative to the world_grid's coordinate system.
    An entity can straddle over multiple world_grids, each world_grid holds a reference to entities that it contains.
    During simulation, the entity's relative position to the simulation's center will be calculated.
*/

struct entity {
    struct world_position center_p;

    struct v2u32 bounding_box_dims;
};

/*

struct world_grid_index {
    struct v2i32 p;
};

struct world_grid {
    struct entity** entities;
    u32 entities_size;
    u32 entities_fill;

    struct world_grid* next; // in case there is a collision in the hash-table that stores it

    struct world_grid_index index;
};

struct world_position {
    struct v2i32 global_p;
    struct v2r32 local_p;
};

struct entity {
    struct world_position center_p;

    struct v2u32 bounding_box_dims;
};

struct world {
    struct world_grid world_grid_hash[4096];
};

*/
