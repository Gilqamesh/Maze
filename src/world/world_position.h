#pragma once

#include "world_defs.h"

#include "../math/v2i32.h"
#include "../math/v2r32.h"

/*
  (0,0)-----------------(1,0)-----------------(2,0)--------------------
    |         .           |                     |                     |
    |         .           |                     |                     |
    |  (-,+)  .  (+,+)    | (-6.4f,87.2f)       |                     |
    |         .           |       ^             |                     |
    |........(0,0)........|       |             |                     |
    |         .           |       |             |                     |
    |  (-,-)  .  (+,-)    |      LOCAL          |                     |
    |         .           |      POSITION       |                     |
    |         .           |                     |                     |
  (0,1)-----------------(1,1)-----------------(2,1)--------------------
    |                     |                     |                     |
    |                     |                     |                     |
    |                     |                     |                     |
    |                     |                     |                     |
    |                     |                     |                     |
    |                     |                     |                     |
    |                     |                     |                     |
    |                     |                     |                     |
    |                     |                     |                     |
  (0,2)-----------------(1,2)-----------------(2,2)--------------------
    |                     |/////////////////////|                     |
    |                     |/////////////////////|                     |
    |                     |/////////////////////|                     |
    |                     |//                 //|                     |
    |                     |// GLOBAL POSITION //|                     |
    |                     |//     (1,2)       //|                     |
    |                     |//                 //|                     |
    |                     |/////////////////////|                     |
    |                     |/////////////////////|                     |
    -------------------------------------------------------------------
*/

struct world_position {
    struct v2i32 global_p;
    struct v2r32 local_p;
};

// @brief returns the relative local position of 'p' relative to 'relative_p'
DLLEXPORT struct v2r32 world_position__to_relative_p(struct world_position p, struct world_position relative_p);
// @brief returns the world position 'p' relative to 'relative_p'
DLLEXPORT struct world_position world_position__from_relative_p(struct v2r32 p, struct world_position relative_p);
// @brief returns the world position after adding 'relative_p' to 'p'
DLLEXPORT struct world_position world_position__add_relative_p(struct world_position p, struct v2r32 relative_p);

static inline struct world_position world_position(struct v2i32 global_p, struct v2r32 local_p) {
    struct world_position result = {
        global_p,
        local_p
    };

    return result;
}
