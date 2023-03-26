#include "sim_region.h"

#include "world_grid.h"

void sim_region__routine(
    struct world* world,
    sim_region__entity_processor_callback entity_processor_callback,
    struct world_position center_p,
    struct v2r32 half_dims
) {
    struct v2r32 world_grid_dims = world_grid__dims();

    struct world_position start_world_p = world_position__from_relative_p(
        v2r32(-half_dims.x, -half_dims.y),
        center_p
    );
    struct world_position end_world_p = world_position__from_relative_p(
        v2r32(half_dims.x, half_dims.y),
        center_p
    );

    for (i32 world_grid_y = start_world_p.global_p.y; world_grid_y <= end_world_p.global_p.y; ++world_grid_y) {
        for (i32 world_grid_x = start_world_p.global_p.x; world_grid_x <= end_world_p.global_p.x; ++world_grid_x) {
            struct v2i32 grid_p = v2i32(world_grid_x, world_grid_y);
            struct world_grid* grid = world__get_grid(world, grid_p);
            struct entity** entities = grid->entities;
            for (u32 entity_index = 0; entity_index < grid->entities_size; ++entity_index) {
                struct entity* entity = entities[entity_index];
                if (entity != NULL) {
                    struct v2r32 entity_p = world_position__to_relative_p(entity->p, center_p);
                    entity->sim_region_relative.relative_p = entity_p;
                    bool entity_is_in_sim_region = v2r32__is_in_half_dims(entity_p, half_dims);
                    entity__flag_set(entity, ENTITY_FLAGS__IS_IN_SIM_REGION, entity_is_in_sim_region);
                }
            }
            entity_processor_callback(entities, grid->entities_size);
            for (u32 entity_index = 0; entity_index < grid->entities_size; ++entity_index) {
                struct entity* entity = entities[entity_index];
                if (entity != NULL) {
                    // note: update entity positions
                    entity->p = world_position__from_relative_p(entity->sim_region_relative.relative_p, center_p);
                    if (v2i32__is_equal(entity->p.global_p, grid_p) == false) {
                        world_grid__remove_entity(grid, entity);
                        struct world_grid* new_grid = world__get_grid(world, entity->p.global_p);
                        world_grid__add_entity(new_grid, entity);
                    }
                }
            }
        }
    }
}
