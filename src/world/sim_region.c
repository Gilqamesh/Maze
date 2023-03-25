#include "sim_region.h"

#include "world_grid.h"

void sim_region__routine(
    struct sim_region* self,
    struct world* world,
    struct renderer* renderer,
    sim_region__entity_processor_callback entity_processor_callback,
    struct world_position p,
    struct v2r32 half_dims
) {
    self->center_p = p;
    self->half_dims = half_dims;

    struct v2r32 world_grid_dims = world_grid__dims();

    struct v2i32 start_world_grid = v2i32(
        (i32) floor((r32) p.global_p.x - (r32) self->half_dims.x / world_grid_dims.x),
        (i32) floor((r32) p.global_p.y - (r32) self->half_dims.y / world_grid_dims.y)
    );
    struct v2i32 end_world_grid = v2i32(
        (i32) floor((r32) p.global_p.x + (r32) self->half_dims.x / world_grid_dims.x),
        (i32) floor((r32) p.global_p.y + (r32) self->half_dims.y / world_grid_dims.y)
    );
    for (i32 world_grid_y = start_world_grid.y; world_grid_y < end_world_grid.y; ++world_grid_y) {
        for (i32 world_grid_x = start_world_grid.x; world_grid_x < end_world_grid.x; ++world_grid_x) {
            struct v2i32 grid_p = v2i32(world_grid_x, world_grid_y);
            struct world_grid* grid = world__get_grid(world, grid_p);
            struct entity** entities = grid->entities;
            for (u32 entity_index = 0; entity_index < grid->entities_size; ++entity_index) {
                struct entity* entity = entities[entity_index];
                if (entity != NULL) {
                    struct v2r32 entity_p = world_position__to_relative_p(entity->p, p);
                    entity->sim_region_relative.relative_p = entity_p;
                    bool entity_is_in_sim_region = v2r32__is_in_half_dims(entity_p, half_dims);
                    entity__flag_set(entity, ENTITY_FLAGS__IS_IN_SIM_REGION, entity_is_in_sim_region);
                    if (renderer != NULL) {
                        bool entity_is_in_render_region = v2r32__is_in_half_dims(entity_p, renderer->half_dims);
                        entity__flag_set(entity, ENTITY_FLAGS__IS_IN_RENDER_REGION, entity_is_in_render_region);
                    }
                }
            }
            entity_processor_callback(entities, renderer, grid->entities_size);
            for (u32 entity_index = 0; entity_index < grid->entities_size; ++entity_index) {
                struct entity* entity = entities[entity_index];
                if (entity != NULL) {
                    // note: update entity positions
                    entity->p = world_position__from_relative_p(entity->sim_region_relative.relative_p, p);
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
