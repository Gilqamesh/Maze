#include "world.h"

void world__create(struct world* world) {
    memset(world->world_grid_hash, 0, sizeof(world->world_grid_hash));
}

void world__destroy(struct world* world) {
}

void world__get_grid(struct world* world, struct world_grid_index index) {
}
