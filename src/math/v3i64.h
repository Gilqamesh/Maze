#pragma once

#include "math_defs.h"

struct v3i64 {
    i64 x;
    i64 y;
    i64 z;
};

static inline struct v3i64 v3i64(i64 x, i64 y, i64 z) {
    struct v3i64 v = {x, y, z};
    return v;
}

static inline struct v3i64 v3i64__scale(struct v3i64 v, i64 s) {
    return v3i64(v.x * s, v.y * s, v.z * s);
}
