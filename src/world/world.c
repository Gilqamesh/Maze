#include "world.h"

void world__create(struct world* self) {
    self->world_grids_size = 4096;
    self->world_grids = (struct world_grid**) calloc(self->world_grids_size, sizeof(self->world_grids));
}

void world__destroy(struct world* self) {
    for (u32 grid_index = 0; grid_index < self->world_grids_size; ++grid_index) {
        world_grid__destroy(self->world_grids[grid_index]);
        free(self->world_grids[grid_index]);
    }

    free(self->world_grids);
}

struct world_grid* world__get_grid(struct world* self, struct v2i32 global_p) {
    u32 hash_index = world_grid__hash(global_p, self->world_grids_size);

    for (u32 grid_index = hash_index; grid_index < self->world_grids_size; ++grid_index) {
        struct world_grid* world_grid = self->world_grids[grid_index];
        if (world_grid != NULL) {
            if (v2i32__is_equal(world_grid->global_p, global_p)) {
                return world_grid;
            }
        } else {
            world_grid = world_grid__create(global_p);
            self->world_grids[grid_index] = world_grid;
            return world_grid;
        }
    }

    for (u32 grid_index = 0; grid_index < hash_index; ++grid_index) {
        struct world_grid* world_grid = self->world_grids[grid_index];
        if (world_grid != NULL) {
            if (v2i32__is_equal(world_grid->global_p, global_p)) {
                return world_grid;
            }
        } else {
            world_grid = world_grid__create(global_p);
            self->world_grids[grid_index] = world_grid;
            return world_grid;
        }
    }

    ExitProcess(APP_ERROR_RAN_OUT_OF_MEMORY);
}
