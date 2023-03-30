#pragma once

#include "math_defs.h"

struct v2i32 {
    i32 x;
    i32 y;
};

static inline struct v2i32 v2i32(i32 x, i32 y) {
    struct v2i32 v = {x, y};
    return v;
}

static inline struct v2i32 v2i32__scale(struct v2i32 v, i32 s) {
    return v2i32(v.x * s, v.y * s);
}

static inline bool v2i32__is_equal(struct v2i32 v, struct v2i32 w) {
    return v.x == w.x && v.y == w.y;
}

static inline struct v2i32 v2i32__sub_v2i32(struct v2i32 v, struct v2i32 w) {
    return v2i32(
        v.x - w.x,
        v.y - w.y
    );
}
