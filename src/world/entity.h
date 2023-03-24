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
    struct world_position p;

    struct v2u32 bounding_box_dims;
};

DLLEXPORT struct entity* entity__create(struct world_position p, struct v2u32 bounding_box_dims);

DLLEXPORT void entity__destroy(struct entity* self);

// @param hash_table_size must be a power of 2
static inline u32 entity__hash(struct entity* self, u32 hash_table_size) {
#pragma warning(push)
#pragma warning(disable: 4311)
    return ((u32) self * 7) & hash_table_size;
#pragma warning(pop)
}

static inline struct v2u32 entity__average_dims(void) {
    return v2u32(
        world__meters_to_pixels(0.2f),
        world__meters_to_pixels(0.2f)
    );
}
