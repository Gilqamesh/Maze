#pragma once

#include "math_defs.h"

struct v3r64 {
    r64 x;
    r64 y;
    r64 z;
};

static inline struct v3r64 v3r64(r64 x, r64 y, r64 z) {
    struct v3r64 v = {x, y, z};
    return v;
}

static inline struct v3r64 v3r64__scale(struct v3r64 v, r64 s) {
    return v3r64(v.x * s, v.y * s, v.z * s);
}
