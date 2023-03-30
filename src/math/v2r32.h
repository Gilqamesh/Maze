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

static inline struct v2r32 v2r32__scale_r32(struct v2r32 v, r32 s) {
    return v2r32(v.x * s, v.y * s);
}

static inline struct v2r32 v2r32__scale_v2r32(struct v2r32 v, struct v2r32 s) {
    return v2r32(
        v.x * s.x,
        v.y * s.y
    );
}

static inline struct v2r32 v2r32__add_v2r32(struct v2r32 v, struct v2r32 w) {
    return v2r32(
        v.x + w.x,
        v.y + w.y
    );
}

static inline struct v2r32 v2r32__sub_v2r32(struct v2r32 v, struct v2r32 w) {
    return v2r32(
        v.x - w.x,
        v.y - w.y
    );
}

static inline r32 v2r32__dot_v2r32(struct v2r32 v, struct v2r32 w) {
    return v.x * w.x + v.y * w.y;
}

static inline r32 v2r32__length(struct v2r32 v) {
    return sqrt(v2r32__dot_v2r32(v, v));
}

static inline struct v2r32 v2r32__normalize(struct v2r32 v) {
    return v2r32__scale_r32(v, 1.0f / v2r32__length(v));
}

static inline bool v2r32__is_in_half_dims(struct v2r32 v, struct v2r32 half_dims) {
    return v.x > -half_dims.x && v.x < half_dims.x && v.y > -half_dims.y && v.y < half_dims.y;
}
