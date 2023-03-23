#pragma once

#include "math_defs.h"

struct v2i64 {
    i64 x;
    i64 y;
};

static inline struct v2i64 v2i64(i64 x, i64 y) {
    struct v2i64 v = {x, y};
    return v;
}

static inline struct v2i64 v2i64__scale(struct v2i64 v, i64 s) {
    return v2i64(v.x * s, v.y * s);
}
