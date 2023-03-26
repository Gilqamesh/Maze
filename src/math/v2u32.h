#pragma once

#include "math_defs.h"

struct v2u32 {
    u32 x;
    u32 y;
};

static inline struct v2u32 v2u32(u32 x, u32 y) {
    struct v2u32 v = {x, y};
    return v;
}

static inline struct v2u32 v2u32__scale(struct v2u32 v, u32 s) {
    return v2u32(v.x * s, v.y * s);
}

static inline bool v2u32__is_less(struct v2u32 v, struct v2u32 relative) {
    return v.x < relative.x && v.y < relative.y;
}
