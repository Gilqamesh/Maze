#include "maze.h"
#include "math/clamp.h"

static inline void _maze_set_entry(struct maze* self, struct v2u32 p, enum maze_entry new_entry) {
    self->data[p.y * self->dims.x + p.x] = (char) new_entry;
}

static inline void _maze_add_step(struct maze* self, struct v2u32 p, enum maze_entry entry) {
    if (self->build_steps.fill < self->build_steps.size) {
        self->build_steps.data[self->build_steps.fill++] = p;
    }
    _maze_set_entry(self, p, entry);
}

static inline void _maze_clear_entries(struct maze* self) {
    char* pmaze = self->data;
    for (u32 row = 0; row < self->dims.y; ++row) {
        for (u32 col = 0; col < self->dims.x; ++col) {
            *(pmaze + col) = MAZE_ENTRY_WALL;
        }
        pmaze += self->dims.x;
    }
}

bool maze__create(struct maze* self, struct v2u32 *dims, u32 seed) {
    *dims = clamp__v2u32(
        v2u32(10, 10),
        *dims,
        v2u32(250, 250)
    );
    self->dims = v2u32(2 * dims->x + 1, 2 * dims->y + 1);
    self->build_steps.size = self->dims.x * self->dims.y;
    self->build_steps.fill = 0;
    self->build_step_count = 0;
    self->seed             = seed;
    self->is_finished      = false;
    self->data = (char*) calloc(self->dims.x * self->dims.y, sizeof(*self->data));
    if (self->data == NULL) {
        return false;
    }

    self->build_steps.data = (struct v2u32*) malloc(self->dims.x * self->dims.y * sizeof(*self->build_steps.data));
    if (self->build_steps.data == NULL) {
        free(self->data);
        return false;
    }

    _maze_clear_entries(self);

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

enum maze_direction {
    MAZE_DIRECTION_UP,
    MAZE_DIRECTION_LEFT,
    MAZE_DIRECTION_RIGHT,
    MAZE_DIRECTION_DOWN,
};

void _maze_dig(struct maze* self, struct v2u32 p) {
    _maze_add_step(self, p, MAZE_ENTRY_ROOM);

    enum maze_direction directions_to_try[4] = {
        MAZE_DIRECTION_UP,
        MAZE_DIRECTION_LEFT,
        MAZE_DIRECTION_RIGHT,
        MAZE_DIRECTION_DOWN
    };

    // shuffle directions
    for (i32 direction_to_try_index = ARRAY_SIZE(directions_to_try) - 1; direction_to_try_index >= 0; --direction_to_try_index) {
        u32 r = rand() % (direction_to_try_index + 1);
        enum maze_direction tmp = directions_to_try[direction_to_try_index];
        directions_to_try[direction_to_try_index] = directions_to_try[r];
        directions_to_try[r] = tmp;
    }

    for (u32 direction_to_try_index = 0; direction_to_try_index < ARRAY_SIZE(directions_to_try); ++direction_to_try_index) {
        // if valid, go there and dig along the way
        switch (directions_to_try[direction_to_try_index]) {
            case MAZE_DIRECTION_UP: {
                if (p.y >= 2 && maze__get_entry(self, v2u32(p.x, p.y - 2)) == MAZE_ENTRY_WALL) {
                    _maze_add_step(self, v2u32(p.x, p.y - 1), MAZE_ENTRY_ROOM);
                    _maze_dig(self, v2u32(p.x, p.y - 2));
                }
            } break ;
            case MAZE_DIRECTION_LEFT: {
                if (p.x >= 2 && maze__get_entry(self, v2u32(p.x - 2, p.y)) == MAZE_ENTRY_WALL) {
                    _maze_add_step(self, v2u32(p.x - 1, p.y), MAZE_ENTRY_ROOM);
                    _maze_dig(self, v2u32(p.x - 2, p.y));
                }
            } break ;
            case MAZE_DIRECTION_DOWN: {
                if (p.y < self->dims.y - 2 && maze__get_entry(self, v2u32(p.x, p.y + 2)) == MAZE_ENTRY_WALL) {
                    _maze_add_step(self, v2u32(p.x, p.y + 1), MAZE_ENTRY_ROOM);
                    _maze_dig(self, v2u32(p.x, p.y + 2));
                }
            } break ;
            case MAZE_DIRECTION_RIGHT: {
                if (p.x < self->dims.x - 2 && maze__get_entry(self, v2u32(p.x + 2, p.y)) == MAZE_ENTRY_WALL) {
                    _maze_add_step(self, v2u32(p.x + 1, p.y), MAZE_ENTRY_ROOM);
                    _maze_dig(self, v2u32(p.x + 2, p.y));
                }
            } break ;
        }
    }
}

void maze__build(struct maze* self) {
    if (maze__is_finished(self)) {
        return ;
    }

    srand(self->seed);
    struct v2u32 maze_entrance = v2u32(0, 1);
    struct v2u32 maze_exit     = v2u32(self->dims.x - 1, self->dims.y - 2);
    _maze_dig(self, v2u32(1, 1));
    
    _maze_clear_entries(self);
    _maze_set_entry(self, maze_entrance, MAZE_ENTRY_ROOM);
    _maze_set_entry(self, maze_exit, MAZE_ENTRY_ROOM);

    self->is_finished = true;
}

enum maze_entry maze__get_entry(struct maze* self, struct v2u32 p) {
    return (enum maze_entry) self->data[p.y * self->dims.x + p.x];
}

bool maze__build_advance(struct maze* self) {
    if (self->build_step_count < self->build_steps.fill) {
        struct v2u32 cur_entry = self->build_steps.data[self->build_step_count];
        _maze_set_entry(self, cur_entry, MAZE_ENTRY_ROOM);
        ++self->build_step_count;
        return true;
    }

    return false;
}

bool maze__is_finished(struct maze* self) {
    return self->is_finished;
}
