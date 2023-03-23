#pragma

#include "world_defs.h"

#include "world_grid.h"

struct world {
    struct world_grid world_grid_hash[4096];
};

void world__create(struct world* world);

void world__destroy(struct world* world);

void world__get_grid(struct world* world, struct world_grid_index index);
