#pragma once

#include "math_defs.h"

struct v3u32 {
    u32 x;
    u32 y;
    u32 z;
};

static inline struct v3u32 v3u32(u32 x, u32 y, u32 z) {
    struct v3u32 v = {x, y, z};
    return v;
}

static inline struct v3u32 v3u32__scale(struct v3u32 v, u32 s) {
    return v3u32(v.x * s, v.y * s, v.z * s);
}
