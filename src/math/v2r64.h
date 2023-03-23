#pragma once

#include "math_defs.h"

struct v2r64 {
    r64 x;
    r64 y;
};

static inline struct v2r64 v2r64(r64 x, r64 y) {
    struct v2r64 v = {x, y};
    return v;
}

static inline struct v2r64 v2r64__scale(struct v2r64 v, r64 s) {
    return v2r64(v.x * s, v.y * s);
}
