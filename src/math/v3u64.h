#pragma once

#include "math_defs.h"

struct v3u64 {
    u64 x;
    u64 y;
    u64 z;
};

static inline struct v3u64 v3u64(u64 x, u64 y, u64 z) {
    struct v3u64 v = {x, y, z};
    return v;
}

static inline struct v3u64 v3u64__scale(struct v3u64 v, u64 s) {
    return v3u64(v.x * s, v.y * s, v.z * s);
}
