#pragma once

#include "math_defs.h"
#include "v2u32.h"

static inline u32 clamp__u32(u32 min, u32 value, u32 max) {
    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }
    return value;
}

static inline struct v2u32 clamp__v2u32(struct v2u32 min, struct v2u32 value, struct v2u32 max) {
    return v2u32(clamp__u32(min.x, value.x, max.x), clamp__u32(min.y, value.y, max.y));
}
