#include "maze.h"

static inline void maze__add_step(struct maze* self) {
}

bool maze__create(struct maze* self, struct v2u32 dims, struct v2u32 start, struct v2u32 end) {
    self->start            = start;
    self->end              = end;
    self->build_steps.dims = dims;
    self->build_steps.fill = 0;
    self->build_step_count = 0;

    self->dims = v2u32(2 * dims.x + 1, 2 * dims.y + 1);
    self->data = (char*) calloc(self->dims.x * self->dims.y, sizeof(*self->data));
    if (self->data == NULL) {
        return false;
    }

    self->build_steps.data = (struct v2u32*) malloc(self->dims.x * self->dims.y * sizeof(*self->build_steps.data));
    if (self->build_steps.data == NULL) {
        free(self->data);
        return false;
    }

    char* pmaze = self->data;
    for (u32 row = 0; row < self->dims.y; ++row) {
        for (u32 col = 0; col < self->dims.x; ++col) {
            *(pmaze + col) = MAZE_ENTRY_WALL;
        }
        pmaze += self->dims.x;
    }

    return true;
}

void maze__destroy(struct maze* self) {
    if (self->data) {
        free(self->data);
    }

    if (self->build_steps.data) {
        free(self->build_steps.data);
    }
}

void maze__build(struct maze* self) {
    if (maze__is_finished(self)) {
        return ;
    }
}

enum maze_entry maze__get_entry(struct maze* self, u32 row, u32 col) {
    return (enum maze_entry) self->data[row * self->dims.x + col];
}

void maze__build_advance(struct maze* self) {
    ++self->build_step_count;
}

bool maze__is_finished(struct maze* self) {
    return self->is_finished;
}
