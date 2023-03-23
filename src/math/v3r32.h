#pragma once

#include "math_defs.h"

struct v3r32 {
    r32 x;
    r32 y;
    r32 z;
};

static inline struct v3r32 v3r32(r32 x, r32 y, r32 z) {
    struct v3r32 v = {x, y, z};
    return v;
}

static inline struct v3r32 v3r32__scale(struct v3r32 v, r32 s) {
    return v3r32(v.x * s, v.y * s, v.z * s);
}
