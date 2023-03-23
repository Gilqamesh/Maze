#pragma once

#include "math_defs.h"

struct v3i32 {
    i32 x;
    i32 y;
    i32 z;
};

static inline struct v3i32 v3i32(i32 x, i32 y, i32 z) {
    struct v3i32 v = {x, y, z};
    return v;
}

static inline struct v3i32 v3i32__scale(struct v3i32 v, i32 s) {
    return v3i32(v.x * s, v.y * s, v.z * s);
}
