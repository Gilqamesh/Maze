#pragma once

#include "math_defs.h"

struct v2u16 {
    u16 x;
    u16 y;
};

static inline struct v2u16 v2u16(u16 x, u16 y) {
    struct v2u16 v = {x, y};
    return v;
}

static inline struct v2u16 v2u16__scale(struct v2u16 v, u16 s) {
    return v2u16(v.x * s, v.y * s);
}
