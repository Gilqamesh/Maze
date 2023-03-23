#pragma once

#include "math_defs.h"

struct v2u64 {
    u64 x;
    u64 y;
};

static inline struct v2u64 v2u64(u64 x, u64 y) {
    struct v2u64 v = {x, y};
    return v;
}

static inline struct v2u64 v2u64__scale(struct v2u64 v, u64 s) {
    return v2u64(v.x * s, v.y * s);
}
