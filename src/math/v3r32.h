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

static inline struct v3r32 v3r32__add_v3r32(struct v3r32 v, struct v3r32 w) {
    return v3r32(
        v.x + w.x,
        v.y + w.y,
        v.z + w.z
    );
}

static inline struct v3r32 v3r32__sub_v3r32(struct v3r32 v, struct v3r32 w) {
    return v3r32(
        v.x - w.x,
        v.y - w.y,
        v.z - w.z
    );
}

static inline struct v3r32 v3r32__scale_r32(struct v3r32 v, r32 s) {
    return v3r32(
        v.x * s,
        v.y * s,
        v.z * s
    );
}

static inline struct v3r32 v3r32__scale_v3r32(struct v3r32 v, struct v3r32 w) {
    return v3r32(
        v.x * w.x,
        v.y * w.y,
        v.z * w.z
    );
}

static inline bool v3r32__is_in_half_dims(struct v3r32 v, struct v3r32 half_dims) {
    return
        v.x > -half_dims.x &&
        v.x < half_dims.x &&
        v.y > -half_dims.y &&
        v.y < half_dims.y &&
        v.z > -half_dims.z &&
        v.z < half_dims.z;
}
