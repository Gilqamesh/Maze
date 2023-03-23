#pragma once

#include "math_defs.h"

struct v2i16 {
    i16 x;
    i16 y;
};

static inline struct v2i16 v2i16(i16 x, i16 y) {
    struct v2i16 v = {x, y};
    return v;
}

static inline struct v2i16 v2i16__scale(struct v2i16 v, i16 s) {
    return v2i16(v.x * s, v.y * s);
}
