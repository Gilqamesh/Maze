#pragma once

#include "math_defs.h"

struct v2r32 {
    r32 x;
    r32 y;
};

static inline struct v2r32 v2r32(r32 x, r32 y) {
    struct v2r32 v = {x, y};
    return v;
}

static inline struct v2r32 v2r32__scale(struct v2r32 v, r32 s) {
    return v2r32(v.x * s, v.y * s);
}

static inline bool v2r32__is_in_half_dims(struct v2r32 v, struct v2r32 half_dims) {
    return v.x > -half_dims.x && v.x < half_dims.x && v.y > -half_dims.y && v.y < half_dims.y;
}
