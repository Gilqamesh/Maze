#include "sim_region.h"

#include "world_grid.h"

void sim_region__routine(
    struct sim_region* self,
    struct world* world,
    sim_region__entity_processor_callback entity_processor_callback,
    struct world_position p,
    struct v2u32 dims
) {
    self->center_p = p;
    self->dims = dims;

    struct v2r32 world_grid_dims = world_grid__dims();

    struct v2i32 start_world_grid = v2i32(
        (i32) floor((r32) p.global_p.x - (r32) self->dims.x / 2.0f),
        (i32) floor((r32) p.global_p.y - (r32) self->dims.y / 2.0f)
    );
    struct v2i32 end_world_grid = v2i32(
        (i32) floor((r32) p.global_p.x + (r32) self->dims.x / 2.0f),
        (i32) floor((r32) p.global_p.y + (r32) self->dims.y / 2.0f)
    );
    for (i32 world_grid_y = start_world_grid.y; world_grid_y < end_world_grid.y; ++world_grid_y) {
        for (i32 world_grid_x = start_world_grid.x; world_grid_x < end_world_grid.x; ++world_grid_x) {
            struct v2i32 grid_p = v2i32(world_grid_x, world_grid_y);
            struct world_grid* grid = world__get_grid(world, grid_p);
            struct entity** entities = grid->entities;
            entity_processor_callback(entities, grid->entities_size, p);
            // note: update entity positions
            for (u32 entity_index = 0; entity_index < grid->entities_size; ++entity_index) {
                struct entity* entity = entities[entity_index];
                if (entity != NULL) {
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
