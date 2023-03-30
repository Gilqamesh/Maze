#pragma once

#include "world_defs.h"

#include "../math/v3u32.h"
#include "../window/color.h"

#include "world_position.h"

/*
    Regarding entity position:
    Has a center point that is relative to the world_grid's coordinate system.
    An entity can straddle over multiple world_grids, each world_grid holds a reference to entities that it contains.
    During simulation, the entity's relative position to the simulation's center will be calculated.
*/

enum entity_flags {
    ENTITY_FLAGS_SIMULATED_ALREADY = 1 << 0,
    ENTITY_FLAGS_IS_IN_SIM_REGION  = 1 << 1
};

struct entity {
    struct {
        struct v3r32 relative_p; // relative to the sim_region's center_p
    } processor_callback_transient_values; // transient values set by set by the sim_region for the entity_processor_callback

    struct v3r32 dp;
    u32 flags;
    struct world_position center_p;
    struct v3r32 bounding_box_half_dims;
    enum color color;
};

DLLEXPORT struct entity* entity__create_absolute(
    struct world_position center_p,
    struct v3r32 bounding_box_half_dims,
    enum color color
);
// @param center_p the entity's position relative to 'relative_p'
// @param relative_p the relative position which 'center_p' is relative to
DLLEXPORT struct entity* entity__create_relative(
    struct v3r32 center_p,
    struct world_position relative_p,
    struct v3r32 bounding_box_half_dims,
    enum color color
);

DLLEXPORT void entity__destroy(struct entity* self);

// @param hash_table_size must be a power of 2
static inline u32 entity__hash(struct entity* self, u32 hash_table_size) {
#pragma warning(push)
#pragma warning(disable: 4311)
    return ((u32) self * 7) & hash_table_size;
#pragma warning(pop)
}

static inline struct v3u32 entity__average_dims(void) {
    return v3u32(
        world__meters_to_pixels(0.2f),
        world__meters_to_pixels(0.2f),
        world__meters_to_pixels(0.2f)
    );
}

static inline void entity__flag_set(struct entity* entity, enum entity_flags flag) {
    entity->flags |= flag;
}

static inline void entity__flag_clear(struct entity* entity, enum entity_flags flag) {
    entity->flags &= ~flag;
}

static inline bool entity__flag_is_set(struct entity* entity, enum entity_flags flag) {
    return (entity->flags & flag) != 0;
}
