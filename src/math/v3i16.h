#pragma once

#include "math_defs.h"

struct v3i16 {
    i16 x;
    i16 y;
    i16 z;
};

static inline struct v3i16 v3i16(i16 x, i16 y, i16 z) {
    struct v3i16 v = {x, y, z};
    return v;
}

static inline struct v3i16 v3i16__scale(struct v3i16 v, i16 s) {
    return v3i16(v.x * s, v.y * s, v.z * s);
}
