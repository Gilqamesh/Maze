#pragma once

#include "defs.h"
#include "math/v2u32.h"
#include "math/v2u16.h"
#include "random/random.h"

enum maze_entry {
    MAZE_ENTRY_WALL   = 'x',
    MAZE_ENTRY_ROOM   = 'o',
    MAZE_ENTRY_ROOM_2 = 'O'
};

struct maze_entries {
    struct v2u16  *data;
    u32           size;
    u32           fill;
};

/*
    x - col
    y - row
*/

/*
    (1, 1) - (7, 1)
    (1, 3) - (7, 3)
    (1, 5) - (7, 5)
    
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
    struct random            random_generator;

    char*                    data; // the continuously building maze between frames
    char*                    render_data; // the currently represented maze
    struct v2u16             dims;

    struct maze_entries      build_steps; // stores each step of the build process

    u32                      build_step_count;
    u32                      unbuild_step_count;
    bool                     is_finished;
};

bool maze__create(struct maze* self, struct v2u16 *dims, u64 seed);
void maze__destroy(struct maze* self);

void maze__build(struct maze* self);

enum maze_entry maze__get_entry(struct maze* self, struct v2u16 p);

// @brief advance the build process by one (purely for animation purposes)
bool maze__build_advance(struct maze* self);

bool maze__is_finished(struct maze* self);
