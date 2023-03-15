#pragma once

#include "defs.h"
#include "math/v2u32.h"

enum maze_entry {
    MAZE_ENTRY_WALL = 'x',
    MAZE_ENTRY_ROOM = 'o'
};

struct maze_build_steps { // stores each step of the build process
    struct v2u32  *data;
    u32           size;
    u32           fill;
};

/*
    x - col
    y - row
*/

/*
    Example: 4x3 maze, every second position is a room
    x x x x x x x x x
    x o x o x o x o x
    x x x x x x x x x
    x o x o x o x o x
    x x x x x x x x x
    x o x o x o x o x
    x x x x x x x x x
*/

struct maze {
    char*                    data;
    struct v2u32             dims;

    struct maze_build_steps  build_steps;
    u32                      build_step_count;

    u32                      seed; // TODO(david): implement a stateful random number generator

    bool                     is_finished;
};

bool maze__create(struct maze* self, struct v2u32 *dims, u32 seed);
void maze__destroy(struct maze* self);

void maze__build(struct maze* self);

enum maze_entry maze__get_entry(struct maze* self, struct v2u32 p);

// @brief advance the build process by one (purely for animation purposes)
bool maze__build_advance(struct maze* self);

bool maze__is_finished(struct maze* self);
