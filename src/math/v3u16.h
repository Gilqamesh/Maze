#pragma once

#include "math_defs.h"

struct v3u16 {
    u16 x;
    u16 y;
    u16 z;
};

static inline struct v3u16 v3u16(u16 x, u16 y, u16 z) {
    struct v3u16 v = {x, y, z};
    return v;
}

static inline struct v3u16 v3u16__scale(struct v3u16 v, u16 s) {
    return v3u16(v.x * s, v.y * s, v.z * s);
}
