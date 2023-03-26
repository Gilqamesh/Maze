#include "world_grid.h"

/*
    Entities hash table must be kept a power of 2 size at all times
*/

#define ENTITIES_START_SIZE 16

struct world_grid* world_grid__create(struct v2i32 global_p) {
    struct world_grid* result = (struct world_grid*) calloc(1, sizeof(*result));

    result->entities_size = ENTITIES_START_SIZE;
    result->entities_fill = 0;
    result->entities = (struct entity**) calloc(result->entities_size, sizeof(*result->entities));
    if (result->entities == NULL) {
        ExitProcess(APP_ERROR_ALLOC_FAIL);
    }

    result->global_p = global_p;

    return result;
}

void world_grid__destroy(struct world_grid* self) {
    free(self->entities);
}

void world_grid__add_entity(struct world_grid* self, struct entity* entity) {
    if (self->entities_size == self->entities_fill) {
        // note: no more space in the hash table to add the entity
        u32 new_entities_size = self->entities_size << 1;
        self->entities = (struct entity**) realloc(self->entities, new_entities_size * sizeof(*self->entities));
        if (self->entities == NULL) {
            ExitProcess(APP_ERROR_ALLOC_FAIL);
        }
        // note: zero out the rest of the hash table
        memset(self->entities + self->entities_size, 0, (new_entities_size - self->entities_size) * sizeof(*self->entities));
        self->entities_size = new_entities_size;
    }

    u32 hash_index = entity__hash(entity, self->entities_size);

    for (u32 entity_index = hash_index; entity_index < self->entities_size; ++entity_index) {
        if (self->entities[entity_index] == NULL) {
            self->entities[entity_index] = entity;
            ++self->entities_fill;
            return ;
        }
        if (self->entities[entity_index] == entity) {
            return ;
        }
    }

    for (u32 entity_index = 0; entity_index < hash_index; ++entity_index) {
        if (self->entities[entity_index] == NULL) {
            self->entities[entity_index] = entity;
            ++self->entities_fill;
            return ;
        }
        if (self->entities[entity_index] == entity) {
            return ;
        }
    }
}

void world_grid__remove_entity(struct world_grid* self, struct entity* entity) {
    u32 hash_index = entity__hash(entity, self->entities_size);

    bool found_entity = false;
    for (u32 entity_index = hash_index; found_entity == false && entity_index < self->entities_size; ++entity_index) {
        if (self->entities[entity_index] == entity) {
            self->entities[entity_index] = NULL;
            --self->entities_fill;
            found_entity = true;
        }
    }

    for (u32 entity_index = 0; found_entity == false && entity_index < hash_index; ++entity_index) {
        if (self->entities[entity_index] == entity) {
            self->entities[entity_index] = NULL;
            --self->entities_fill;
            found_entity = true;
        }
    }

    if (self->entities_size > ENTITIES_START_SIZE && self->entities_fill == (self->entities_size >> 1)) {
        u32 new_entities_size = self->entities_size >> 1;
        // note: move the entities from the upper half to the lower half
        for (u32 entity_index = self->entities_fill; entity_index < self->entities_size; ++entity_index) {
            struct entity* cur_entity = self->entities[entity_index];
            u32 cur_hash_index = entity__hash(cur_entity, new_entities_size);

            bool found_slot = false;
            for (u32 new_entity_index = cur_hash_index; found_slot == false && new_entity_index < new_entities_size; ++new_entity_index) {
                if (self->entities[new_entity_index] == NULL) {
                    self->entities[new_entity_index] = cur_entity;
                    found_slot = true;
                }
            }

            for (u32 new_entity_index = 0; found_slot == false && new_entity_index < cur_hash_index; ++new_entity_index) {
                if (self->entities[new_entity_index] == NULL) {
                    self->entities[new_entity_index] = cur_entity;
                    found_slot = true;
                }
            }
            assert(found_slot == true);
        }

        // remove the upper half of the hash table as it's now copied to fill the lower half
        self->entities = (struct entity**) realloc(self->entities, new_entities_size * sizeof(*self->entities));
        self->entities_size = new_entities_size;
    }
}
