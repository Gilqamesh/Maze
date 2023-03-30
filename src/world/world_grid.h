#pragma once

#include "world_defs.h"

#include "entity.h"
#include "../math/v3i32.h"

/*
    Regarding world_grid's implicit coordinate system:
    Center point is (0, 0, 0).
*/
struct world_grid {
    struct entity** entities; // hash table
    u32 entities_size; // hash table size
    u32 entities_fill;

    struct v3i32 global_p;
};

static inline struct v3r32 world_grid__dims(void) {
    return v3r32(
        world__meters_to_pixels(16.0f),
        world__meters_to_pixels(16.0f),
        world__meters_to_pixels(16.0f)
    );
}

DLLEXPORT struct world_grid* world_grid__create(struct v3i32 global_p);

DLLEXPORT void world_grid__destroy(struct world_grid* self);

// @brief adds an entity to the world_grid
DLLEXPORT void world_grid__add_entity(struct world_grid* self, struct entity* entity);
// @brief removes an entity from the world_grid
DLLEXPORT void world_grid__remove_entity(struct world_grid* self, struct entity* entity);

// @param hash_table_size must be a power of 2
static inline u32 world_grid__hash(struct v3i32 global_p, u32 hash_table_size) {
    return ((u32) global_p.x * 7 + (u32) global_p.y * 9 + (u32) global_p.z * 11) & hash_table_size;
}
