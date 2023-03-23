#include "maze.h"
#include "math/clamp.h"

static inline void _maze_set_entry(struct maze* self, char* data, struct v2u16 p, enum maze_entry new_entry) {
    data[(u32) p.y * (u32) self->dims.x + (u32) p.x] = (char) new_entry;
}

static inline enum maze_entry _maze_get_entry(struct maze* self, char* data, struct v2u16 p) {
    return (enum maze_entry) data[(u32) p.y * (u32) self->dims.x + (u32) p.x];
}

static inline void _maze_add_step(struct maze* self, char* data, struct v2u16 p, enum maze_entry entry) {
    if (self->build_steps.fill < self->build_steps.size) {
        if (_maze_get_entry(self, data, p) == MAZE_ENTRY_WALL) {
            _maze_set_entry(self, data, p, entry);
            self->build_steps.data[self->build_steps.fill++] = p;
        }
    }
}

static inline void _maze_clear_entries(struct maze* self, char* data) {
    const enum maze_entry entry_dispatch[2] = {
        MAZE_ENTRY_WALL,
        MAZE_ENTRY_ROOM
    };

    memset(data, MAZE_ENTRY_WALL, self->dims.x * self->dims.y);

    // data += self->dims.x;
    // for (u32 row_iter = 1; row_iter < self->dims.y; row_iter += 2) {
    //     for (u32 col_iter = 1; col_iter < self->dims.x; col_iter += 2) {
    //         *(data + col_iter) = MAZE_ENTRY_ROOM;
    //     }
    //     data += self->dims.x;
    //     data += self->dims.x;
    // }

    // for (u32 row = 1; row < self->dims.y - 1; ++row) {
    //     for (u32 col = 1; col < self->dims.x - 1; ++col) {
    //         *(data + col) = entry_dispatch[col & 0b1];
    //     }
    //     data += self->dims.x;
    // }
}

bool maze__create(struct maze* self, struct v2u16 *dims, u64 seed) {
    random__init(&self->random_generator, seed);

    *dims = clamp__v2u16(
        v2u16(10, 10),
        *dims,
        v2u16(250, 250)
    );
    self->dims               = v2u16(2 * (u32) dims->x + 1, 2 * (u32) dims->y + 1);
    self->is_finished        = false;
    self->build_step_count   = 0;
    self->unbuild_step_count = 0;

    self->data = (char*) calloc((u32) self->dims.x * (u32) self->dims.y, sizeof(*self->data));
    if (self->data == NULL) {
        return false;
    }

    self->render_data = (char*) calloc((u32) self->dims.x * (u32) self->dims.y, sizeof(*self->render_data));
    if (self->render_data == NULL) {
        return false;
    }

    self->build_steps.size = (u32) self->dims.x * (u32) self->dims.y;
    self->build_steps.fill = 0;
    self->build_steps.data = (struct v2u16*) malloc(self->build_steps.size * sizeof(*self->build_steps.data));
    if (self->build_steps.data == NULL) {
        free(self->data);
        return false;
    }

    _maze_clear_entries(self, self->data);
    _maze_clear_entries(self, self->render_data);

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

void _maze_dig(struct maze* self, struct v2u16 p, struct v2u16 end) {
    _maze_add_step(self, self->data, p, MAZE_ENTRY_ROOM);

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
                if (p.y >= 2 && _maze_get_entry(self, self->data, v2u16(p.x, p.y - 2)) == MAZE_ENTRY_WALL) {
                    _maze_add_step(self, self->data, v2u16(p.x, p.y - 1), MAZE_ENTRY_ROOM);
                    _maze_dig(self, v2u16(p.x, p.y - 2), end);
                }
            } break ;
            case MAZE_DIRECTION_LEFT: {
                if (p.x >= 2 && _maze_get_entry(self, self->data, v2u16(p.x - 2, p.y)) == MAZE_ENTRY_WALL) {
                    _maze_add_step(self, self->data, v2u16(p.x - 1, p.y), MAZE_ENTRY_ROOM);
                    _maze_dig(self, v2u16(p.x - 2, p.y), end);
                }
            } break ;
            case MAZE_DIRECTION_DOWN: {
                if (p.y < self->dims.y - 2 && _maze_get_entry(self, self->data, v2u16(p.x, p.y + 2)) == MAZE_ENTRY_WALL) {
                    _maze_add_step(self, self->data, v2u16(p.x, p.y + 1), MAZE_ENTRY_ROOM);
                    _maze_dig(self, v2u16(p.x, p.y + 2), end);
                } else if (p.y < self->dims.y - 1 && p.x == end.x && p.y + 1 == end.y) {
                    _maze_add_step(self, self->data, end, MAZE_ENTRY_ROOM);
                }
            } break ;
            case MAZE_DIRECTION_RIGHT: {
                if (p.x < self->dims.x - 2 && _maze_get_entry(self, self->data, v2u16(p.x + 2, p.y)) == MAZE_ENTRY_WALL) {
                    _maze_add_step(self, self->data, v2u16(p.x + 1, p.y), MAZE_ENTRY_ROOM);
                    _maze_dig(self, v2u16(p.x + 2, p.y), end);
                } else if (p.x < self->dims.x - 1 && p.x + 1 == end.x && p.y == end.y) {
                    _maze_add_step(self, self->data, end, MAZE_ENTRY_ROOM);
                }
            } break ;
        }
    }
}

static inline enum maze_direction maze_direction__opposite(enum maze_direction direction) {
    switch (direction) {
        case MAZE_DIRECTION_UP: return MAZE_DIRECTION_DOWN;
        case MAZE_DIRECTION_LEFT: return MAZE_DIRECTION_RIGHT;
        case MAZE_DIRECTION_RIGHT: return MAZE_DIRECTION_LEFT;
        case MAZE_DIRECTION_DOWN: return MAZE_DIRECTION_UP;
    }

    return 0;
}

struct build_path { // to store the randomly build path so far
    enum maze_direction  built_from;
    enum maze_direction  direction;
    i32                  left_neighbour_index;
    i32                  right_neighbour_index;
    i32                  up_neighbour_index;
    i32                  down_neighbour_index;
};

static inline void build_path__clear(struct build_path* build_path, u32 size) {
    for (u32 build_path_index = 0; build_path_index < size; ++build_path_index) {
        build_path[build_path_index].down_neighbour_index  = -1;
        build_path[build_path_index].up_neighbour_index    = -1;
        build_path[build_path_index].left_neighbour_index  = -1;
        build_path[build_path_index].right_neighbour_index = -1;
    }
}

void maze__build2(struct maze* self) {
    // if (maze__is_finished(self)) {
    //     return ;
    // }

    struct v2u16 maze_entrance = v2u16(0, 1);
    struct v2u16 maze_exit = v2u16(0, 1);
    _maze_add_step(self, self->data, maze_entrance, MAZE_ENTRY_ROOM);
    _maze_add_step(self, self->data, maze_exit, MAZE_ENTRY_ROOM);

    struct maze_entries unvisited_entries;
    unvisited_entries.fill = 0;
    unvisited_entries.size = (self->dims.x >> 1) * (self->dims.y >> 1);
    // TODO(david): error checking
    unvisited_entries.data = (struct v2u16*) malloc(sizeof(*unvisited_entries.data) * unvisited_entries.size);
    if (unvisited_entries.data == NULL) {
        ExitProcess(APP_ERROR_ALLOC_FAIL);
    }

    for (u32 row_index = 1; row_index < self->dims.y; row_index += 2) {
        for (u32 col_index = 1; col_index < self->dims.x; col_index += 2) {
            unvisited_entries.data[unvisited_entries.fill++] = v2u16(col_index, row_index);
        }
    }

    // shuffle unvisited entries
    for (i32 unvisited_entry_index = unvisited_entries.size - 1; unvisited_entry_index >= 0; --unvisited_entry_index) {
        u32 r = rand() % (unvisited_entry_index + 1);
        struct v2u16 tmp = unvisited_entries.data[unvisited_entry_index];
        unvisited_entries.data[unvisited_entry_index] = unvisited_entries.data[r];
        unvisited_entries.data[r] = tmp;
    }

    u32 build_path_size = (self->dims.x >> 1) * (self->dims.y >> 1);
    struct build_path *build_path = (struct build_path* ) malloc(sizeof(*build_path) * build_path_size);
    u32 build_path_fill = 0;
    build_path__clear(build_path, build_path_size);

    struct v2u16 start_entry = unvisited_entries.data[--unvisited_entries.fill];
    _maze_add_step(self, self->data, start_entry, MAZE_ENTRY_ROOM);
    while (unvisited_entries.fill > 0) {
        start_entry = unvisited_entries.data[--unvisited_entries.fill];

        enum maze_direction random_direction = (enum maze_direction) random__u16_closed(&self->random_generator, 0, 3);
        if (build_path_fill > 0) {
            while (build_path[build_path_fill - 1].built_from == maze_direction__opposite(random_direction)) {
                random_direction = (enum maze_direction) random__u16_closed(&self->random_generator, 0, 3);
            }
        }
        build_path[build_path_fill].built_from = maze_direction__opposite(random_direction);
        build_path[build_path_fill].direction = random_direction;
        ++build_path_fill;

        // after finding the random direction

        // do random walk until we'd hit a room
    }

    self->is_finished = true;
}

void maze__build(struct maze* self) {
    if (maze__is_finished(self)) {
        return ;
    }

    struct v2u16 start_from;
    if (self->build_steps.fill == 0) {
        struct v2u16 maze_entrance = v2u16(0, 1);
        _maze_add_step(self, self->data, maze_entrance, MAZE_ENTRY_ROOM);
        start_from = v2u16(1, 1);
    } else {
        start_from = self->build_steps.data[self->build_steps.fill - 1];
    }

    struct v2u16 maze_exit = v2u16(self->dims.x - 1, self->dims.y - 2);
    _maze_dig(self, start_from, maze_exit);

    // when is this finished?
    self->is_finished = true;
}

enum maze_entry maze__get_entry(struct maze* self, struct v2u16 p) {
    return (enum maze_entry) self->render_data[(u32) p.y * (u32) self->dims.x + (u32) p.x];
}

bool maze__build_advance(struct maze* self) {
    if (self->build_step_count < self->build_steps.fill) {
        struct v2u16 cur_entry = self->build_steps.data[self->build_step_count];
        _maze_set_entry(self, self->render_data, cur_entry, MAZE_ENTRY_ROOM);
        ++self->build_step_count;
        ++self->unbuild_step_count;
        return true;
    } else if (self->unbuild_step_count > 0) {
        --self->unbuild_step_count;
        struct v2u16 cur_entry = self->build_steps.data[self->unbuild_step_count];
        _maze_set_entry(self, self->render_data, cur_entry, MAZE_ENTRY_ROOM_2);
        return true;
    }

    return false;
}

bool maze__is_finished(struct maze* self) {
    return self->is_finished;
}
