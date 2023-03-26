#pragma once

#include "math_defs.h"

#include "v2u32.h"

static inline u32 abs_diff__u32(u32 a, u32 b) {
    if (a > b) {
        return a - b;
    }

    return b - a;
}

static inline struct v2u32 abs_diff__v2u32(struct v2u32 v, struct v2u32 w) {
    return v2u32(
        abs_diff__u32(v.x, w.x),
        abs_diff__u32(v.y, w.y)
    );
}
