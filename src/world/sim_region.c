#include "sim_region.h"

#include "world_grid.h"

void sim_region__routine(
    struct world* world,
    sim_region__entity_processor_callback entity_processor_callback,
    void* entity_processor_callback_context,
    struct world_position center_p,
    struct v2r32 half_dims
) {
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

            // note: pull in entities from the grid that are inside the sim_region's bounds
            for (u32 entity_index = 0; entity_index < grid->entities_size; ++entity_index) {
                struct entity* entity = entities[entity_index];
                if (entity != NULL) {
                    struct v2r32 entity_p = world_position__to_relative_p(entity->center_p, center_p);
                    entity->processor_callback_transient_values.relative_p = entity_p;
                    entity->processor_callback_transient_values.is_in_sim_region = v2r32__is_in_half_dims(entity_p, half_dims);
                }
            }

            // note: process the entities with a callback
            entity_processor_callback(entity_processor_callback_context, entities, grid->entities_size);

            // note: update entity world_positions, clear transient flags
            for (u32 entity_index = 0; entity_index < grid->entities_size; ++entity_index) {
                struct entity* entity = entities[entity_index];
                if (entity != NULL) {
                    entity->center_p = world_position__from_relative_p(entity->processor_callback_transient_values.relative_p, center_p);
                    entity->processor_callback_transient_values.is_in_sim_region = false;
                    if (v2i32__is_equal(entity->center_p.global_p, grid_p) == false) {
                        world_grid__remove_entity(grid, entity);
                        struct world_grid* new_grid = world__get_grid(world, entity->center_p.global_p);
                        world_grid__add_entity(new_grid, entity);
                    }
                }
            }
        }
    }
}
